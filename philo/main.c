/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 17:51:48 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/24 08:24:36 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_philosophers(t_args *args, t_philo **philos)
{
	int	i;

	*philos = malloc(sizeof(t_philo) * args->n_philo);
	if (!*philos)
	{
		printf("Error: Failed to allocate memory for philosophers\n");
		free(args->forks);
		return (-1);
	}
	i = 0;
	while (i < args->n_philo)
	{
		(*philos)[i].id = i + 1;
		(*philos)[i].ate_count = 0;
		(*philos)[i].last_meal = get_time();
		(*philos)[i].left_fork = &args->forks[i];
		(*philos)[i].right_fork = &args->forks[(i + 1) % args->n_philo];
		(*philos)[i].args = args;
		i++;
	}
	return (0);
}

static int	create_philo_threads(t_args *args, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args->n_philo)
	{
		if (pthread_create(&philos[i].thread, NULL, philo_routine,
				(void *)&philos[i]) != 0)
		{
			printf("Error: Failed to create philosopher thread\n");
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	create_and_join_all_threads(t_args *args, t_philo *philos)
{
	pthread_t	monitor_thread;
	int			i;

	if (create_philo_threads(args, philos) == -1)
		return (-1);
	if (pthread_create(&monitor_thread, NULL, monitor_routine,
			(void *)philos) != 0)
	{
		printf("Error: Failed to create monitor thread\n");
		return (-1);
	}
	pthread_join(monitor_thread, NULL);
	i = 0;
	while (i < args->n_philo)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	return (0);
}

static void	cleanup(t_args *args, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < args->n_philo)
	{
		pthread_mutex_destroy(&args->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&args->print_mutex);
	pthread_mutex_destroy(&args->death_mutex);
	free(philos);
	free(args->forks);
}

int	main(int argc, char const *argv[])
{
	t_args	args;
	t_philo	*philos;

	if (init_args(argc, argv, &args) == -1)
		return (1);
	if (init_mutexes(&args) == -1)
		return (1);
	if (init_philosophers(&args, &philos) == -1)
		return (1);
	if (create_and_join_all_threads(&args, philos) == -1)
		return (1);
	cleanup(&args, philos);
	return (0);
}
