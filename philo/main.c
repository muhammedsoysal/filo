/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:51:48 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/20 10:32:59 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

static int	args_check(int argc, char const *argv[])
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0 && ft_is_digit(argv[i][0]))
			return (0);
		i++;
	}
	return (1);
}
int main(int argc, char const *argv[])
{
	t_args		args;
	t_philo		*philos;
	pthread_t	monitor_thread;
	int			i;

	i = 0;
	// Initialize the arguments
	if (args_check(argc, argv) == 0 || argc < 5 || argc > 6)
	{
		printf("Error: Invalid arguments\n");
		return (1);
	}
	// Parse the arguments
	args.n_philo = ft_atoi(argv[1]);
	args.time_to_die = ft_atoi(argv[2]);
	args.time_to_eat = ft_atoi(argv[3]);
	args.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		args.n_must_eat = ft_atoi(argv[5]);
	else
		args.n_must_eat = -1; // No limit on eating
	// Initialize shared variables
	args.start_time = get_time();
	args.one_died = 0;
	args.finished_eating = 0;
	// Initialize the mutexes
	args.forks = malloc(sizeof(pthread_mutex_t) * args.n_philo);
	if (!args.forks)
		return (printf("Error: Failed to allocate memory for forks\n"), 1);
	if (pthread_mutex_init(&args.print_mutex, NULL) != 0)
		return (free(args.forks), printf("Error: Failed to initialize print mutex\n"), 1);
	if (pthread_mutex_init(&args.death_mutex, NULL) != 0)
		return (free(args.forks), printf("Error: Failed to initialize death mutex\n"), 1);
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
	// Initialize the philosophers
	i = 0;
	philos = malloc(sizeof(t_philo) * args.n_philo);
	if (!philos)
		return (free(args.forks), printf("Error: Failed to allocate memory for philosophers\n"), 1);
	while (i < args.n_philo)
	{
		philos[i].id = i + 1; // ID başlangıcı 1'den
		philos[i].ate_count = 0;
		philos[i].last_meal = get_time();
		philos[i].left_fork = &args.forks[i];
		philos[i].right_fork = &args.forks[(i + 1) % args.n_philo];
		philos[i].args = &args;
		i++;
	}
	// Create philosopher threads
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
	// Create monitor thread
	if (pthread_create(&monitor_thread, NULL, monitor_routine, (void *)philos) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (1);
	}
	// Wait for all philosophers to finish
	i = 0;
	while (i < args.n_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor_thread, NULL);
	// Free allocated memory and destroy mutexes
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
