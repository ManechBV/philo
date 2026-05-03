/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:28:51 by mabenois          #+#    #+#             */
/*   Updated: 2026/05/03 03:13:08 by mabenois         ###   ########.fr       */
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

typedef struct	s_times
{
	long long	die;
	long long	eat;
	long long	sleep;
}	t_times;

typedef struct	s_philo
{
	pthread_t		thread;
	int				id;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	*fork_r;
	t_times			*times;
	pthread_mutex_t	*start_mut;
	pthread_mutex_t	*print_mut;
	pthread_mutex_t	*death_mut;
	int				*should_die;
	int				is_dead;
	long long		time;
	long long		time_start;
	long long		time_last_meal;
}	t_philo;

typedef struct	s_table
{
	long long		nb_philos;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	t_times			*times;
	pthread_mutex_t	start_mut;
	pthread_mutex_t	print_mut;
	pthread_mutex_t	death_mut;
	int				should_die;
}	t_table;

//	utils.c
long long		ft_atol(char *s);

//	init_table_forks.c
void			ft_assign_forks(t_table *table, long long i);
pthread_mutex_t	*ft_create_forks(long long nb_philos);
t_table			*ft_create_table(char **av);

//	init_philos.c
int				ft_init_philos(t_table *table);
int				ft_init_mutexes(t_table *table);
void			ft_assign_forks(t_table *table, long long i);
void			ft_set_mutexes_ptr(t_table *table, long long i);

//	philos_routine.c
void			*philo_routine(void *curr);
void			ft_philo_wait_for_start(t_philo *self);
void			philo_print(t_philo *curr, char *message);
int				philo_eat(t_philo *curr);
int				philo_sleep(t_philo *curr);

//	philos_forks.c
void			philo_take_forks(t_philo *curr);
void			philo_release_forks(t_philo *curr);

#endif
