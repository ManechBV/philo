/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:28:51 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/12 02:24:22 by mabenois         ###   ########.fr       */
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

typedef struct	s_philo
{
	pthread_t		thread;
	int				id;
	int				is_last;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t	*print_mut;
	pthread_mutex_t	*death_mut;
	pthread_mutex_t	*time_start_mut;
	pthread_mutex_t	meal_mutex;
	int				*should_die;
	long long		*time_die;
	long long		*time_eat;
	long long		*time_sleep;
	long long		*time_start;
	long long		time_last_meal;
	long long		meals_eaten;
	long long		*max_meals;
}	t_philo;

typedef struct	s_table
{
	long long		nb_philos;
	t_philo			*philos;
	pthread_t		monitor;
	pthread_mutex_t	*forks;
	long long		time_die;
	long long		time_start;
	long long		time_eat;
	long long		time_sleep;
	long long		max_meals;
	pthread_mutex_t	print_mut;
	pthread_mutex_t	death_mut;
	pthread_mutex_t	time_start_mut;
	int				should_die;
}	t_table;

//	utils.c
long long		ft_atol(char *s);

#endif
