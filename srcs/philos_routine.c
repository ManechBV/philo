/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <mabenois@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:55:05 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/05 13:54:54 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_get_time(void)
{
	struct timeval	time;
	long long		curr_time;

	if (gettimeofday(&time, NULL) != 0)
		return (-1);
	curr_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (curr_time);
}

static int	ft_philo_should_termiate(t_philo *self)
{
	pthread_mutex_lock(self->death_mut);
	if (*(self->should_die) == 1)
	{
		pthread_mutex_unlock(self->death_mut);
		return (-1);
	}
	pthread_mutex_unlock(self->death_mut);
	return (0);
}

void	*philo_routine(void *curr)
{
	t_philo	*self;

	self = (t_philo*)curr;
	ft_philo_wait_for_start(self);
	while (1)
	{
		//if (self->id % 2 == 0)
		//	usleep(1000);
		/*
		if (ft_philo_should_termiate(self) == -1 || philo_eat(self) == -1)
			break ;
		if (ft_philo_should_termiate(self) == -1 || philo_sleep(self) == -1)
			break ;
			*/
		if (philo_eat(self) == -1)
			break ;
		if (philo_sleep(self) == -1)
			break ;
		if (ft_philo_should_termiate(self) == -1)
			break ;
		philo_print(self, "\e[0;36mis thinking", 0);
	}
	return (NULL);
}

void	ft_philo_wait_for_start(t_philo *self)
{
	struct timeval	time;

	if (gettimeofday(&time, NULL) != 0)
		return ;
	pthread_mutex_lock(&self->meal_mutex);
	self->time_start = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	self->time_last_meal = self->time_start;
	pthread_mutex_unlock(&self->meal_mutex);
	return ;
}

void	philo_print(t_philo *curr, char *message, int death_mess)
{
	struct timeval	time;
	long long		curr_time;

	if (gettimeofday(&time, NULL) != 0)
	{
		return ;
	}
	curr_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	if (ft_philo_should_termiate(curr) == -1 && death_mess == 0)
		return ;
	pthread_mutex_lock(curr->print_mut);
	printf("\e[1;33m%lld \e[0;33mphilo \e[0;34m%d %s\n", curr_time - curr->time_start, curr->id, message);
	pthread_mutex_unlock(curr->print_mut);
}

void	ft_philo_set_eat_times(t_philo *curr)
{
		pthread_mutex_lock(&curr->meal_mutex);
		curr->time_last_meal = ft_get_time();
		curr->meals_eaten++;
		pthread_mutex_unlock(&curr->meal_mutex);
}

int	philo_eat(t_philo *curr)
{
	philo_take_forks(curr);
	pthread_mutex_lock(&curr->meal_mutex);
	if ((ft_get_time() - curr->time_last_meal + curr->time_eat) > curr->time_die
			|| (curr->fork_l == curr->fork_r))
	{
		pthread_mutex_unlock(&curr->meal_mutex);
		if (curr->fork_l != curr->fork_r)
			philo_print(curr, "\e[0;32mis eating", 0);
		ft_philo_set_eat_times(curr);
		usleep(curr->time_die * 1000);
		philo_release_forks(curr);
		return (-1);
	}
	pthread_mutex_unlock(&curr->meal_mutex);
	philo_print(curr, "\e[0;32mis eating", 0);
	ft_philo_set_eat_times(curr);
	usleep(curr->time_eat * 1000);
	philo_release_forks(curr);
	return (0);
}

int	philo_sleep(t_philo *curr)
{
	pthread_mutex_lock(&curr->meal_mutex);
	if ((ft_get_time() - curr->time_last_meal + curr->time_sleep) > curr->time_die)
	{
		pthread_mutex_unlock(&curr->meal_mutex);
		philo_print(curr, "\e[1;34mis sleeping", 0);
		usleep(curr->time_die * 1000);
		return (-1);
	}
	pthread_mutex_unlock(&curr->meal_mutex);
	philo_print(curr, "\e[1;34mis sleeping", 0);
	usleep(curr->time_sleep * 1000);
	return (0);
}
