/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_two.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:16:47 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/24 10:27:04 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_action(t_philo *philo, char *action)
{
	long	time;

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

int	is_dead(t_philo *philo)
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

void	take_forks(t_philo *philo)
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

void	sleep_and_think(t_philo *philo)
{
	print_action(philo, "is sleeping");
	ft_usleep(philo->args->time_to_sleep);
	print_action(philo, "is thinking");
}

int	check_eating_complete(t_philo *p)
{
	pthread_mutex_lock(&p->args->death_mutex);
	if (p->args->n_must_eat != -1 && p->ate_count >= p->args->n_must_eat)
	{
		p->args->finished_eating++;
		pthread_mutex_unlock(&p->args->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&p->args->death_mutex);
	return (0);
}
