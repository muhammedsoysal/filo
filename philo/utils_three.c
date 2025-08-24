/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_three.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:16:47 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/24 07:51:44 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	handle_philo_death(t_args *args, t_philo *philos, int i)
{
	args->one_died = 1;
	pthread_mutex_unlock(&args->death_mutex);
	pthread_mutex_lock(&args->print_mutex);
	printf("%ld %d died\n", get_time() - args->start_time, philos[i].id);
	pthread_mutex_unlock(&args->print_mutex);
}

int	check_all_ate_enough(t_args *args)
{
	if (args->n_must_eat != -1 && args->finished_eating >= args->n_philo)
	{
		args->one_died = 1;
		pthread_mutex_unlock(&args->death_mutex);
		printf("all philo ate enough\n");
		return (1);
	}
	return (0);
}

int	init_args(int argc, char const *argv[], t_args *args)
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

int	init_mutexes(t_args *args)
{
	int	i;

	if (pthread_mutex_init(&args->print_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize print mutex\n");
		free(args->forks);
		return (-1);
	}
	if (pthread_mutex_init(&args->death_mutex, NULL) != 0)
	{
		printf("Error: Failed to initialize death mutex\n");
		return (free(args->forks), -1);
	}
	i = 0;
	while (i < args->n_philo)
	{
		if (pthread_mutex_init(&args->forks[i], NULL) != 0)
		{
			printf("Error: Failed to initialize mutex\n");
			free(args->forks);
			return (-1);
		}
		i++;
	}
	return (0);
}
