/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:16:47 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/23 23:36:31 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	eat(t_philo *philo)
{
	take_forks(philo);
	if (philo->args->n_philo == 1)
		return ;
	pthread_mutex_lock(&philo->args->death_mutex);
	philo->last_meal = get_time();
	philo->ate_count++;
	pthread_mutex_unlock(&philo->args->death_mutex);
	print_action(philo, "is eating");
	ft_usleep(philo->args->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
}

void	*philo_routine(void *philo)
{
	t_philo	*p;

	p = (t_philo *)philo;
	if (p->id % 2 == 0)
		ft_usleep(p->args->time_to_eat / 2);
	while (!is_dead(p))
	{
		if (is_dead(p))
			break ;
		eat(p);
		if (p->args->n_philo == 1)
			break ;
		if (check_eating_complete(p))
			break ;
		if (is_dead(p))
			break ;
		sleep_and_think(p);
		usleep(100);
	}
	return (NULL);
}

void	*monitor_routine(void *arg)
{
	t_philo	*philos;
	t_args	*args;
	int		i;

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
				handle_philo_death(args, philos, i);
				return (NULL);
			}
			if (check_all_ate_enough(args))
				return (NULL);
			pthread_mutex_unlock(&args->death_mutex);
			i++;
		}
		usleep(1000);
	}
	return (NULL);
}
