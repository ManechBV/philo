/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table_forks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:40:24 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 22:45:19 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_create_times(t_table *table, int ac, char **av)
{
	table->time_die = ft_atol(av[2]);
	table->time_eat = ft_atol(av[3]);
	table->time_sleep = ft_atol(av[4]);
	table->max_meals = -1;
	if (ac > 5)
		table->max_meals = ft_atol(av[5]);
	if (table->time_die == -1 || table->time_eat == -1
		|| table->time_sleep == -1)
		return (-1);
	return (0);
}

t_fork	*ft_create_forks(long long nb_philos)
{
	t_fork		*forks;
	long long	i;

	forks = malloc(sizeof(t_fork) * nb_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < nb_philos)
	{
		if (pthread_mutex_init(&(forks[i].mut), NULL) != 0
			|| pthread_mutex_init(&(forks[i].taken_mut), NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(forks[i].mut));
			free(forks);
			return (NULL);
		}
		forks[i].taken = 0;
		i++;
	}
	return (forks);
}

t_table	*ft_create_table(int ac, char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	table->nb_philos = ft_atol(av[1]);
	if (table->nb_philos < 1 || ft_create_times(table, ac, av) != 0)
	{
		free(table);
		return (NULL);
	}
	table->forks = ft_create_forks(table->nb_philos);
	if (!table->forks)
	{
		free(table);
		return (NULL);
	}
	return (table);
}

void	ft_free_table(t_table *table)
{
	long long	i;
	t_philo		*curr;

	i = 0;
	while (i < table->nb_philos)
	{
		curr = &table->philos[i];
		pthread_mutex_destroy(&curr->meal_mutex);
		pthread_mutex_destroy(&(table->forks[i].mut));
		pthread_mutex_destroy(&(table->forks[i].taken_mut));
		i++;
	}
	free(table->philos);
	free(table->forks);
	pthread_mutex_destroy(&table->death_mut);
	pthread_mutex_destroy(&table->print_mut);
	free(table);
}
