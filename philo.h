#ifndef PHILO_H
#define PHILO_H

#include <stdio.h>	  // printf
#include <stdlib.h>	  // malloc, free, atoi
#include <unistd.h>	  // usleep
#include <sys/time.h> // gettimeofday
#include <pthread.h>  // pthread_create, mutexler

typedef struct s_args
{
	int n_philo;			// toplam filozof sayısı
	int time_to_die;		// bir filozofun ölmesi için geçen süre
	int time_to_eat;		// yemek yeme süresi
	int time_to_sleep;		// uyku süresi
	int n_must_eat;			// her filozofun yemesi gereken minimum miktar
	long start_time;		// programın başlangıç zamanı
	pthread_mutex_t *forks; // çatal mutexleri
	pthread_mutex_t print_mutex; // yazdırma için mutex
	pthread_mutex_t death_mutex; // ölüm kontrolü için mutex
	int one_died;			// bir filozofun öldüğünü kontrol etmek için
	int finished_eating;	// yemek yemeyi bitiren filozof sayısı
} t_args;
typedef struct s_philo t_philo;

typedef struct s_philo
{
	int id;						 // filozofun id si
	int ate_count;				 // yediği yemek sayısı
	long last_meal;				 // son yediği yemek zamanı
	pthread_t thread;			 // filozofun thread'i
	pthread_mutex_t *left_fork;	 // sol çatal
	pthread_mutex_t *right_fork; // sağ çatal
	t_args *args;				 // argümanlar
} t_philo;

// Fonksiyon prototipleri (ileride tanımlayacağız)
long get_time(void);
void ft_usleep(long time);
int ft_atoi(const char *str);
void *philo_routine(void *philo);
int is_dead(t_philo *philo);
int all_ate_enough(t_args *args);
void print_action(t_philo *philo, char *action);
void *monitor_routine(void *arg);
#endif