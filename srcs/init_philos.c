/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:44:29 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/05 13:14:10 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

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

static void	*monitor_routine(void *data)
{
	t_table		*table;
	t_philo		*curr;
	long long	i;
	long long	philos_eat_max;

	table = (t_table*) data;
	while (1)
	{
		i = 0;
		philos_eat_max = 0;
		while (i < table->nb_philos)
		{
			curr = &table->philos[i];
			pthread_mutex_lock(curr->death_mut);
			pthread_mutex_lock(&(curr->meal_mutex));
			if (curr->meals_eaten >= table->max_meals && table->max_meals > 0)
				philos_eat_max++;
			if (((ft_get_time() - curr->time_last_meal) > table->time_die))
			{
				pthread_mutex_unlock(&(curr->meal_mutex));
				table->should_die = 1;
				pthread_mutex_unlock(curr->death_mut);
				philo_print(curr, "\e[0;91mdied", 1);
				ft_monitor_join_philos(table);
				return (NULL);
			}
			pthread_mutex_unlock(&(curr->meal_mutex));
			pthread_mutex_unlock(curr->death_mut);
			i++;
		}
		if (philos_eat_max == i)
		{
				pthread_mutex_lock(curr->death_mut);
				table->should_die = 1;
				pthread_mutex_unlock(curr->death_mut);
				ft_monitor_join_philos(table);
				return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

static int	ft_init_monitor(t_table *table)
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

int	ft_init_philos(t_table *table)
{
	long long	i;

	table->philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!table->philos)
		return (-1);
	i = 0;
	while (i < table->nb_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].time_die = table->time_die;
		table->philos[i].time_eat = table->time_eat;
		table->philos[i].time_sleep = table->time_sleep;
		table->philos[i].time_last_meal = ft_get_time();
		table->philos[i].meals_eaten = 0;
		table->philos[i].max_meals = table->max_meals;
		table->philos[i].is_last = 0;
		if (i == table->nb_philos - 1)
			table->philos[i].is_last = 1;
		if (pthread_mutex_init(&(table->philos[i].meal_mutex), NULL) != 0)
			return (-1);
		ft_assign_forks(table, i);
		ft_set_mutexes_ptr(table, i);
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine, &table->philos[i]) != 0)
			return (-1);
		i++;
	}
	if (ft_init_monitor(table) != 0)
		return (-1);
	return (0);
}

int	ft_init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->print_mut, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&table->death_mut, NULL) != 0)
		return (-1);
	return (0);
}

void	ft_assign_forks(t_table *table, long long i)
{
	long long	fork_r_id;

	fork_r_id = i + 1;
	if (fork_r_id >= table->nb_philos)
		fork_r_id = 0;
	table->philos[i].fork_l = &table->forks[i];
	table->philos[i].fork_r = &table->forks[fork_r_id];
}

void	ft_set_mutexes_ptr(t_table *table, long long i)
{
	table->philos[i].print_mut = &table->print_mut;
	table->philos[i].death_mut = &table->death_mut;
	table->philos[i].should_die = &table->should_die;
}
