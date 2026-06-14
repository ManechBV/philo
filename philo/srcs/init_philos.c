/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:44:29 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 23:14:30 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

static void	ft_init_curr_philo(t_table *table, long long i)
{
	table->philos[i].id = i + 1;
	table->philos[i].time_die = table->time_die;
	table->philos[i].time_start = &table->time_start;
	table->philos[i].time_eat = table->time_eat;
	table->philos[i].time_sleep = table->time_sleep;
	table->philos[i].time_last_meal = -1;
	table->philos[i].meals_eaten = 0;
	table->philos[i].max_meals = table->max_meals;
	table->philos[i].is_last = 0;
	if (i == table->nb_philos - 1)
		table->philos[i].is_last = 1;
}

int	ft_init_philos(t_table *table)
{
	long long	i;

	table->philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!table->philos)
		return (-1);
	table->time_start = ft_get_time();
	i = 0;
	while (i < table->nb_philos)
	{
		ft_init_curr_philo(table, i);
		if (pthread_mutex_init(&(table->philos[i].meal_mutex), NULL) != 0)
			return (-1);
		ft_assign_forks(table, i);
		ft_set_mutexes_ptr(table, i);
		if (i % 2 == 0)
			usleep(1000);
		if (pthread_create(&table->philos[i].thread, NULL,
				philo_routine, &table->philos[i]) != 0)
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

	fork_r_id = i - 1;
	if (fork_r_id < 0)
		fork_r_id = table->nb_philos - 1;
	if (i % 2 != 0)
	{
		table->philos[i].fork_l = &table->forks[i];
		table->philos[i].fork_r = &table->forks[fork_r_id];
		return ;
	}
	table->philos[i].fork_r = &table->forks[i];
	table->philos[i].fork_l = &table->forks[fork_r_id];
}

void	ft_set_mutexes_ptr(t_table *table, long long i)
{
	table->philos[i].print_mut = &table->print_mut;
	table->philos[i].death_mut = &table->death_mut;
	table->philos[i].should_die = &table->should_die;
}
