/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:28:51 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 23:12:21 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/select.h>
# include <sys/time.h>

typedef struct s_fork
{
	pthread_mutex_t	mut;
	pthread_mutex_t	taken_mut;
	char			taken;
}	t_fork;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				is_last;
	t_fork			*fork_l;
	t_fork			*fork_r;
	pthread_mutex_t	*start_mut;
	pthread_mutex_t	*print_mut;
	pthread_mutex_t	*death_mut;
	pthread_mutex_t	meal_mutex;
	int				*should_die;
	long long		time_die;
	long long		time_eat;
	long long		time_sleep;
	long long		*time_start;
	long long		time_last_meal;
	long long		meals_eaten;
	long long		max_meals;
}	t_philo;

typedef struct s_table
{
	long long		nb_philos;
	t_philo			*philos;
	pthread_t		monitor;
	t_fork			*forks;
	long long		time_die;
	long long		time_start;
	long long		time_eat;
	long long		time_sleep;
	long long		max_meals;
	pthread_mutex_t	print_mut;
	pthread_mutex_t	death_mut;
	int				should_die;
}	t_table;

//	utils.c
long long		ft_atol(char *s);
long long		ft_get_time(void);
void			ft_philo_set_eat_times(t_philo *curr);
int				ft_philo_should_termiate(t_philo *self);

//	init_table_forks.c
void			ft_assign_forks(t_table *table, long long i);
t_fork			*ft_create_forks(long long nb_philos);
t_table			*ft_create_table(int ac, char **av);
void			ft_free_table(t_table *table);

//	init_philos.c
int				ft_init_philos(t_table *table);
int				ft_init_mutexes(t_table *table);
void			ft_assign_forks(t_table *table, long long i);
void			ft_set_mutexes_ptr(t_table *table, long long i);

//	philos_routine.c
void			*philo_routine(void *curr);
void			ft_philo_wait_for_start(t_philo *self);
void			philo_print(t_philo *curr, char *message, int death_mess);
int				philo_eat(t_philo *curr);
int				philo_sleep(t_philo *curr, long long duration, int mess);

//	philos_forks.c
void			philo_take_forks(t_philo *curr);
void			philo_release_forks(t_philo *curr);

//	monitor.c
int				ft_init_monitor(t_table *table);

#endif
