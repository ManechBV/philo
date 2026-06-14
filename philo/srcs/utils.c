/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:36:08 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 23:12:06 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	ft_atol(char *s)
{
	long long	i;
	long long	ret;

	i = 0;
	ret = 0;
	if (!s)
		return (-1);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (-1);
		ret = (ret * 10) + (s[i] - '0');
		i++;
	}
	return (ret);
}

long long	ft_get_time(void)
{
	struct timeval	time;
	long long		curr_time;

	if (gettimeofday(&time, NULL) != 0)
		return (-1);
	curr_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (curr_time);
}

void	ft_philo_set_eat_times(t_philo *curr)
{
	pthread_mutex_lock(&curr->meal_mutex);
	curr->time_last_meal = ft_get_time();
	curr->meals_eaten++;
	pthread_mutex_unlock(&curr->meal_mutex);
}

int	ft_philo_should_termiate(t_philo *self)
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
