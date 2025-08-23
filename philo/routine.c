/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:16:47 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/23 17:50:19 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_action(t_philo *philo, char *action)
{
	long time;

	pthread_mutex_lock(&philo->args->print_mutex);
	pthread_mutex_lock(&philo->args->death_mutex);
	if (!philo->args->one_died)
	{
		time = get_time() - philo->args->start_time;
		printf("%ld %d %s\n", time, philo->id, action);
	}
	pthread_mutex_unlock(&philo->args->death_mutex);
	pthread_mutex_unlock(&philo->args->print_mutex);
}

int is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->args->death_mutex);
	if (philo->args->one_died)
	{
		pthread_mutex_unlock(&philo->args->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo->args->death_mutex);
	return (0);
}

void take_forks(t_philo *philo)
{
	if (philo->args->n_philo == 1)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_unlock(philo->left_fork);
		return ;
	}

	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
	}
	else
	{
		usleep(100);
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork");
	}
}

void eat(t_philo *philo)
{
	take_forks(philo);

	if (philo->args->n_philo == 1)
		return;
	pthread_mutex_lock(&philo->args->death_mutex);
	philo->last_meal = get_time();
	philo->ate_count++;
	pthread_mutex_unlock(&philo->args->death_mutex);
	print_action(philo, "is eating");
	ft_usleep(philo->args->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void sleep_and_think(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->args->time_to_sleep);
	print_action(philo, "is thinking");
}

void *philo_routine(void *philo)
{
	t_philo *p;

	p = (t_philo *)philo;
	if (p->id % 2 == 0)
		ft_usleep(p->args->time_to_eat / 2);
	while (!is_dead(p))
	{
		if (is_dead(p))
			break;
		eat(p);
		if (p->args->n_philo == 1)
			break;
		pthread_mutex_lock(&p->args->death_mutex);
		if (p->args->n_must_eat != -1 && p->ate_count >= p->args->n_must_eat)
		{
			p->args->finished_eating++;
			pthread_mutex_unlock(&p->args->death_mutex);
			break;
		}
		pthread_mutex_unlock(&p->args->death_mutex);
		if (is_dead(p))
			break;
		sleep_and_think(p);
		usleep(100);
	}
	return (NULL);
}

void *monitor_routine(void *arg)
{
	t_philo *philos;
	t_args *args;
	int i;

	philos = (t_philo *)arg;
	args = philos[0].args;
	while (1)
	{
		i = 0;
		while (i < args->n_philo)
		{
			pthread_mutex_lock(&args->death_mutex);
			if (get_time() - philos[i].last_meal > args->time_to_die)
			{
				args->one_died = 1;
				pthread_mutex_unlock(&args->death_mutex);
				pthread_mutex_lock(&args->print_mutex);
				printf("%ld %d died\n", get_time() - args->start_time, philos[i].id);
				pthread_mutex_unlock(&args->print_mutex);
				return (NULL);
			}
			if (args->n_must_eat != -1 && args->finished_eating >= args->n_philo)
			{
				args->one_died = 1;
				pthread_mutex_unlock(&args->death_mutex);
				printf("all philo ate enough\n");
				return (NULL);
			}
			pthread_mutex_unlock(&args->death_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
