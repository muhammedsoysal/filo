/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 18:16:47 by musoysal          #+#    #+#             */
/*   Updated: 2025/04/15 13:40:41 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
void *philo_routine(void *philo)
{
	t_philo *p = (t_philo *)philo;
	t_args *args = p->args;

	while (1)
	{
		// Ölüm kontrolü: Eğer filozof uzun süre yemek yemediyse ölecek
		if (get_time() - p->last_meal > args->time_to_die)
		{
			printf("Philosopher %d died\n", p->id);
			args->one_died = 1; // Bir filozof öldü
			break;
		}

		// Yemek yeme
		pthread_mutex_lock(p->left_fork);
		pthread_mutex_lock(p->right_fork);
		p->last_meal = get_time(); // Son yemek zamanını güncelle
		p->ate_count++;			   // Yemek yediğini say

		printf("time: %ld Philosopher %d is eating \n", p->last_meal, p->id);
		ft_usleep(args->time_to_eat);
		pthread_mutex_unlock(p->left_fork);
		pthread_mutex_unlock(p->right_fork);

		// Eğer her filozof belirli bir sayıda yemek yediyse durur
		if (args->n_must_eat != -1 && p->ate_count >= args->n_must_eat)
			break;

		// Uyuma
		printf("Philosopher %d is sleeping\n", p->id);
		ft_usleep(args->time_to_sleep);
	}

	return (NULL);
}