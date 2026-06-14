/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_routine.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <mabenois@student.42angouleme.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:55:05 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 18:46:48 by mabenois         ###   ########.fr       */
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
	/*
	if (self->is_last == 1 && self->id % 2 != 0)
		usleep(1000 + (self->is_last * 1000));
		*/
	//printf("-- philo %d, r: %p l: %p\n", self->id, self->fork_r, self->fork_l);
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
		if (ft_philo_should_termiate(self) == -1)
			break ;
		if (philo_eat(self) == -1)
			break ;
		if (philo_sleep(self, self->time_sleep, 1) == -1)
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
	self->time_last_meal = *(self->time_start);
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
	printf("\e[1;33m%lld \e[0;33mphilo \e[0;34m%d %s\n", curr_time - *(curr->time_start), curr->id, message);
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
	/*
	pthread_mutex_lock(&curr->meal_mutex);
	if ((ft_get_time() - curr->time_last_meal + curr->time_eat) > curr->time_die
			|| (curr->fork_l == curr->fork_r))
	{
		pthread_mutex_unlock(&curr->meal_mutex);
		if (curr->fork_l != curr->fork_r)
			philo_print(curr, "\e[0;32mis eating dead", 0);
		ft_philo_set_eat_times(curr);
		philo_sleep(curr, curr->time_die, 0);
		philo_release_forks(curr);
		return (-1);
	}
	pthread_mutex_unlock(&curr->meal_mutex);
	*/
	if (curr->fork_l == curr->fork_r)
	{
		philo_release_forks(curr);
		if (philo_sleep(curr, curr->time_die + 500, 0) == -1)
			return (-1);
		return (0);
	}
	philo_print(curr, "\e[0;32mis eating", 0);
	ft_philo_set_eat_times(curr);
	if (philo_sleep(curr, curr->time_eat, 0) == -1)
	{
		philo_release_forks(curr);
		return (-1);
	}
	philo_release_forks(curr);
	return (0);
}

int	philo_sleep(t_philo *curr, long long duration, int mess)
{
	long long	start_sleep_time;

	/*
	pthread_mutex_lock(&curr->meal_mutex);
	if ((ft_get_time() - curr->time_last_meal + curr->time_sleep) > (curr->time_die * 1000))
	{
		pthread_mutex_unlock(&curr->meal_mutex);
		philo_print(curr, "\e[1;34mis sleeping dead", 0);
		usleep(curr->time_die * 1000);
		return (-1);
	}
	pthread_mutex_unlock(&curr->meal_mutex);
	*/
	if (mess == 1)
		philo_print(curr, "\e[1;34mis sleeping", 0);
	start_sleep_time = ft_get_time();
	while ((ft_get_time() - start_sleep_time) <= duration)
	{
		usleep(500);
		if (ft_philo_should_termiate(curr) == -1)
			return (-1);
	}
	return (0);
}
