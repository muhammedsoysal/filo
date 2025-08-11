/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:16:47 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/10 19:46:37 by musoysal         ###   ########.fr       */
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
		print_action(philo, "has taken a fork 🍽️");
		ft_usleep(philo->args->time_to_die);
		pthread_mutex_unlock(philo->left_fork);
		return;
	}
	// Deadlock'u önlemek için çift ID'li filozoflar önce sol, tek ID'liler önce sağ çatalı alır
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork 🍽️");
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork 🍽️");
	}
	else
	{
		pthread_mutex_lock(philo->right_fork);
		print_action(philo, "has taken a fork 🍽️");
		pthread_mutex_lock(philo->left_fork);
		print_action(philo, "has taken a fork 🍽️");
	}
}

void eat(t_philo *philo)
{
	take_forks(philo);
	
	pthread_mutex_lock(&philo->args->death_mutex);
	philo->last_meal = get_time();
	philo->ate_count++;
	pthread_mutex_unlock(&philo->args->death_mutex);
	
	print_action(philo, "is eating 🥘");
	ft_usleep(philo->args->time_to_eat);
	
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void sleep_and_think(t_philo *philo)
{
	print_action(philo, "is sleeping 💤");
	ft_usleep(philo->args->time_to_sleep);
	print_action(philo, "is thinking 🤔");
}

void *philo_routine(void *philo)
{
	t_philo *p = (t_philo *)philo;

	// Çift ID'li filozofların biraz beklemesini sağla (deadlock'u önlemek için)
	if (p->id % 2 == 0)
		ft_usleep(p->args->time_to_eat / 2);

	while (!is_dead(p))
	{
		eat(p);
		
		// Yeterince yediyse döngüden çık
		pthread_mutex_lock(&p->args->death_mutex);
		if (p->args->n_must_eat != -1 && p->ate_count >= p->args->n_must_eat)
		{
			p->args->finished_eating++;
			pthread_mutex_unlock(&p->args->death_mutex);
			break;
		}
		pthread_mutex_unlock(&p->args->death_mutex);
		
		sleep_and_think(p);
	}
	return (NULL);
}

void *monitor_routine(void *arg)
{
	t_philo *philos = (t_philo *)arg;
	t_args *args = philos[0].args;
	int i;

	while (1)
	{
		i = 0;
		while (i < args->n_philo)
		{
			pthread_mutex_lock(&args->death_mutex);
			
			// Ölüm kontrolü
			if (get_time() - philos[i].last_meal > args->time_to_die)
			{
				args->one_died = 1;
				pthread_mutex_unlock(&args->death_mutex);
				pthread_mutex_lock(&args->print_mutex);
				printf("%ld %d died\n", get_time() - args->start_time, philos[i].id);
				pthread_mutex_unlock(&args->print_mutex);
				return (NULL);
			}
			
			// Herkes yeterince yediyse bitir
			if (args->n_must_eat != -1 && args->finished_eating >= args->n_philo)
			{
				args->one_died = 1;
				pthread_mutex_unlock(&args->death_mutex);
				return (NULL);
			}
			
			pthread_mutex_unlock(&args->death_mutex);
			i++;
		}
		usleep(1000); // 1ms bekle
	}
	return (NULL);
}
