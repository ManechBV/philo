/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/14 22:20:45 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 23:03:52 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_monitor_join_philos(t_table *table)
{
	long long	i;

	i = 0;
	while (i < table->nb_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
}

static int	ft_monitor_check_death(t_table *table, long long i)
{
	while (i < table->nb_philos)
	{
		pthread_mutex_lock(&((&table->philos[i])->meal_mutex));
		if ((&table->philos[i])->time_last_meal == -1)
		{
			pthread_mutex_unlock(&((&table->philos[i])->meal_mutex));
			i++;
			continue ;
		}
		pthread_mutex_lock(table->philos[i].death_mut);
		if (((ft_get_time() - table->philos[i].time_last_meal)
				> table->time_die))
		{
			pthread_mutex_unlock(&(table->philos[i].meal_mutex));
			table->should_die = 1;
			pthread_mutex_unlock(table->philos[i].death_mut);
			philo_print(&table->philos[i], "\e[0;91mdied", 1);
			ft_monitor_join_philos(table);
			return (-1);
		}
		pthread_mutex_unlock(table->philos[i].death_mut);
		pthread_mutex_unlock(&table->philos[i].meal_mutex);
		i++;
	}
	return (0);
}

static int	ft_monitor_check_meals(t_table *table)
{
	long long	philos_eat_max;
	long long	i;
	t_philo		*curr;

	philos_eat_max = 0;
	i = 0;
	while (i < table->nb_philos)
	{
		curr = &table->philos[i];
		if (curr->meals_eaten >= table->max_meals && table->max_meals > 0)
			philos_eat_max++;
		i++;
	}
	if (philos_eat_max == i)
	{
		pthread_mutex_lock(curr->death_mut);
		table->should_die = 1;
		pthread_mutex_unlock(curr->death_mut);
		ft_monitor_join_philos(table);
		return (-1);
	}
	return (0);
}

static void	*monitor_routine(void *data)
{
	t_table		*table;

	table = (t_table *) data;
	while (1)
	{
		if (ft_monitor_check_death(table, 0) == -1)
			return (NULL);
		if (ft_monitor_check_meals(((t_table *) data)) == -1)
			break ;
		usleep(800);
	}
	return (NULL);
}

int	ft_init_monitor(t_table *table)
{
	if (pthread_create(&table->monitor, NULL, monitor_routine, table) != 0)
	{
		pthread_mutex_lock(&table->death_mut);
		table->should_die = 1;
		pthread_mutex_unlock(&table->death_mut);
		return (-1);
	}
	return (0);
}
