/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:55:05 by mabenois          #+#    #+#             */
/*   Updated: 2026/05/03 03:11:02 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine(void *curr)
{
	t_philo	*self;

	self = (t_philo*)curr;
	//philo_print(self, "created, waiting for start");
	ft_philo_wait_for_start(self);
	//philo_print(self, "started");
	while (1)
	{
		if (philo_eat(self) == -1)
			break ;
		philo_release_forks(self);
		if (philo_sleep(self) == -1)
			break ;
		philo_print(self, "\e[0;36mis thinking");
	}
	return (NULL);
}

void	ft_philo_wait_for_start(t_philo *self)
{
	struct timeval	time;

	pthread_mutex_lock(self->start_mut);
	pthread_mutex_unlock(self->start_mut);
	if (gettimeofday(&time, NULL) != 0)
		return ;
	self->time_start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	self->time_last_meal = self->time_start;
	return ;
}

void	philo_print(t_philo *curr, char *message)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
	{
		pthread_mutex_unlock(curr->print_mut);
		return ;
	}
	curr->time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	pthread_mutex_lock(curr->print_mut);
	printf("\e[1;33m%lld \e[0;33mphilo \e[0;34m%d %s\n", curr->time - curr->time_start, curr->id, message);
	pthread_mutex_unlock(curr->print_mut);
}

int	philo_eat(t_philo *curr)
{
	philo_take_forks(curr);
	pthread_mutex_lock(curr->death_mut);
	philo_print(curr, "\e[0;32mis eating");
	curr->time_last_meal = curr->time;
	if (curr->times->die < curr->times->eat || *curr->should_die == 1)
	{
		if (*curr->should_die == 0)
		{
			*curr->should_die = 1;
			pthread_mutex_unlock(curr->death_mut);
			usleep(curr->times->die * 1000);
			philo_release_forks(curr);
			philo_print(curr, "\e[0;31mdied");
			return (-1);
		}
		pthread_mutex_unlock(curr->death_mut);
		return (-1);
	}
	pthread_mutex_unlock(curr->death_mut);
	usleep(curr->times->eat * 1000);
	return (0);
}

int	philo_sleep(t_philo *curr)
{
	pthread_mutex_lock(curr->death_mut);
	philo_print(curr, "\e[0;32mis sleeping");
	if (curr->time - curr->time_last_meal + curr->times->sleep > curr->times->die || *curr->should_die == 1)
	{
		if (*curr->should_die == 0)
		{
			*curr->should_die = 1;
			pthread_mutex_unlock(curr->death_mut);
			usleep(curr->times->die * 1000);
			philo_print(curr, "\e[0;31mdied");
			return (-1);
		}
		pthread_mutex_unlock(curr->death_mut);
		return (-1);
	}
	pthread_mutex_unlock(curr->death_mut);
	usleep(curr->times->sleep * 1000);
	return (0);
}
