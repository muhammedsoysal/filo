/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:51:48 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/23 17:57:45 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_args(int argc, char const *argv[], t_args *args)
{
	if (args_check(argc, argv) == 0 || argc < 5 || argc > 6)
	{
		printf("Error: Invalid arguments\n");
		return (-1);
	}
	args->n_philo = ft_atoi(argv[1]);
	args->time_to_die = ft_atoi(argv[2]);
	args->time_to_eat = ft_atoi(argv[3]);
	args->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args->n_must_eat = ft_atoi(argv[5]);
	else
		args->n_must_eat = -1;
	args->start_time = get_time();
	args->one_died = 0;
	args->finished_eating = 0;
	args->forks = malloc(sizeof(pthread_mutex_t) * args->n_philo);
	if (!args->forks)
	{
		printf("Error: Failed to allocate memory for forks\n");
		return (-1);
	}
	return (0);
}

int main(int argc, char const *argv[])
{
	t_args		args;
	t_philo		*philos;
	pthread_t	monitor_thread;
	int			i;

	if (init_args(argc, argv, &args) == -1)
		return (1);
	if (pthread_mutex_init(&args.print_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize print mutex\n");
		free(args.forks);
		return (1);
	}
	if (pthread_mutex_init(&args.death_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize death mutex\n");
		free(args.forks);
		return (1);
	}
	i = 0;
	while (i < args.n_philo)
	{
		if (pthread_mutex_init(&args.forks[i], NULL) != 0)
		{
			printf("Error: Failed to initialize mutex\n");
			free(args.forks);
			return (1);
		}
		i++;
	}
	i = 0;
	philos = malloc(sizeof(t_philo) * args.n_philo);
	if (!philos)
		return (free(args.forks), printf("Error: Failed to allocate memory for philosophers\n"), 1);
	while (i < args.n_philo)
	{
		philos[i].id = i + 1;
		philos[i].ate_count = 0;
		philos[i].last_meal = get_time();
		philos[i].left_fork = &args.forks[i];
		philos[i].right_fork = &args.forks[(i + 1) % args.n_philo];
		philos[i].args = &args;
		i++;
	}
	i = 0;
	while (i < args.n_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine, (void *)&philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread\n");
			return (1);
		}
		i++;
	}
	if (pthread_create(&monitor_thread, NULL, monitor_routine, (void *)philos) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}
	i = 0;
	while (i < args.n_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < args.n_philo)
	{
		pthread_mutex_destroy(&args.forks[i]);
		i++;
	}
	pthread_mutex_destroy(&args.print_mutex);
	pthread_mutex_destroy(&args.death_mutex);
	return (free(philos), free(args.forks), 0);
}
