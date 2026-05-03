/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_table_forks.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:40:24 by mabenois          #+#    #+#             */
/*   Updated: 2026/05/03 01:41:20 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_times	*ft_create_times(char **av)
{
	t_times	*times;

	times = malloc(sizeof(t_times));
	if (!times)
		return (NULL);
	times->die = ft_atol(av[2]);
	times->eat = ft_atol(av[3]);
	times->sleep = ft_atol(av[4]);
	if (times->die == -1 || times->eat == -1 || times->sleep == -1)
	{
		free(times);
		return (NULL);
	}
	return (times);
}

pthread_mutex_t	*ft_create_forks(long long nb_philos)
{
	pthread_mutex_t	*forks;
	long long		i;

	forks = malloc(sizeof(pthread_mutex_t) * nb_philos);
	if (!forks)
		return (NULL);
	i = 0;
	while (i < nb_philos)
	{
		if (pthread_mutex_init(&forks[i], NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&forks[i]);
			free(forks);
			return (NULL);
		}
		i++;
	}
	return (forks);
}

t_table	*ft_create_table(char **av)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	if (!table)
		return (NULL);
	table->nb_philos = ft_atol(av[1]);
	if (table->nb_philos < 1)
	{
		free(table);
		return (NULL);
	}
	table->times = ft_create_times(av);
	if (!table->times)
	{
		free(table);
		return (NULL);
	}
	table->forks = ft_create_forks(table->nb_philos);
	if (!table->forks)
	{
		free(table->times);
		free(table);
		return (NULL);
	}
	return (table);
}
