/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philos.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:44:29 by mabenois          #+#    #+#             */
/*   Updated: 2026/05/03 03:17:59 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_init_philos(t_table *table)
{
	long long	i;

	table->philos = malloc(sizeof(t_philo) * table->nb_philos);
	if (!table->philos)
		return (-1);
	i = 0;
	pthread_mutex_lock(&table->start_mut);
	while (i < table->nb_philos)
	{
		table->philos[i].id = i + 1;
		ft_assign_forks(table, i);
		ft_set_mutexes_ptr(table, i);
		table->philos[i].times = table->times;
		if (pthread_create(&table->philos[i].thread, NULL, philo_routine, &table->philos[i]) != 0)
		{
			// TODO un mutex dans table qui permet de dire aux philo quils devrait return
			return (-1);
		}
		i++;
	}
	usleep(100);
	pthread_mutex_unlock(&table->start_mut);
	return (0);
}

int	ft_init_mutexes(t_table *table)
{
	if (pthread_mutex_init(&table->start_mut, NULL) != 0)
		return (-1);
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
	table->philos[i].start_mut = &table->start_mut;
	table->philos[i].print_mut = &table->print_mut;
	table->philos[i].death_mut = &table->death_mut;
	table->philos[i].should_die = &table->should_die;
	table->philos[i].is_dead = 0;
}
