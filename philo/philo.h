/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: musoysal <musoysal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 22:28:49 by musoysal          #+#    #+#             */
/*   Updated: 2025/08/24 17:51:12 by musoysal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_args
{
	int				n_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_must_eat;
	long			start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	int				one_died;
	int				finished_eating;
}	t_args;

typedef struct s_philo
{
	int				id;
	int				ate_count;
	long			last_meal;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	t_args			*args;
}	t_philo;

long	get_time(void);
void	ft_usleep(long time);
int		ft_atoi(const char *str);
void	*philo_routine(void *philo);
int		is_dead(t_philo *philo);
int		all_ate_enough(t_args *args);
void	print_action(t_philo *philo, char *action);
void	*monitor_routine(void *arg);
int		args_check(int argc, char const *argv[]);
void	take_forks(t_philo *philo);
void	sleep_and_think(t_philo *philo);
int		check_eating_complete(t_philo *p);
void	handle_philo_death(t_args *args, t_philo *philos, int i);
int		check_all_ate_enough(t_args *args);
int		init_args(int argc, char const *argv[], t_args *args);
int		init_mutexes(t_args *args, int i);

#endif