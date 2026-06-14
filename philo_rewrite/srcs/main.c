/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/12 01:41:28 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/14 15:12:45 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

t_table	*ft_init_table(int ac, char **av)
{
	long long	i;
	t_table		*new;
	t_philo		*curr;

	new = malloc(sizeof(t_table));
	if (!new)
		return (NULL);
	new->nb_philos = ft_atol(av[1]);
	new->time_die = ft_atol(av[2]);
	new->time_eat = ft_atol(av[3]);
	new->time_sleep = ft_atol(av[4]);
	new->max_meals = -1;
	new->time_start = -1;
	if (ac == 6)
		new->max_meals = ft_atol(av[5]);
	if (pthread_mutex_init(&new->death_mut, NULL) != 0)
	{
		free(new);
		return (NULL);
	}
	if (pthread_mutex_init(&new->print_mut, NULL) != 0)
	{
		pthread_mutex_destroy(&new->death_mut);
		free(new);
		return (NULL);
	}
	if (pthread_mutex_init(&new->time_start_mut, NULL) != 0)
	{
		pthread_mutex_destroy(&new->print_mut);
		pthread_mutex_destroy(&new->death_mut);
		free(new);
		return (NULL);
	}
	new->forks = malloc(sizeof(pthread_mutex_t) * new->nb_philos);
	if (!new)
	{
		pthread_mutex_destroy(&new->time_start_mut);
		pthread_mutex_destroy(&new->print_mut);
		pthread_mutex_destroy(&new->death_mut);
		free(new);
		return (NULL);
	}
	i = 0;
	while (i < new->nb_philos)
	{
		if (pthread_mutex_init(&(new->forks[i]), NULL) != 0)
		{
			while (--i >= 0)
				pthread_mutex_destroy(&(new->forks[i]));
			pthread_mutex_destroy(&new->time_start_mut);
			pthread_mutex_destroy(&new->print_mut);
			pthread_mutex_destroy(&new->death_mut);
			free(new);
			return (NULL);
		}
		i++;
	}
	new->philos = malloc(sizeof(t_philo) * new->nb_philos);
	if (!(new->philos))
	{
		i = new->nb_philos;
		while (--i >= 0)
			pthread_mutex_destroy(&(new->forks[i]));
		pthread_mutex_destroy(&new->time_start_mut);
		pthread_mutex_destroy(&new->print_mut);
		pthread_mutex_destroy(&new->death_mut);
		free(new->forks);
		free(new);
		return (NULL);
	}
	i = 0;
	while (i < new->nb_philos)
	{
		curr = &new->philos[i];
		curr->death_mut = &new->death_mut;
		curr->print_mut = &new->print_mut;
		curr->time_start_mut = &new->time_start_mut;
		curr->id = i;
		curr->is_last = 0;
		curr->meals_eaten = 0;
		if (i == new->nb_philos - 1)
			curr->is_last = 1;
		curr->max_meals = &new->max_meals;
		curr->should_die = &new->should_die;
		curr->time_die = &new->time_die;
		curr->time_eat = &new->time_eat;
		curr->time_sleep = &new->time_sleep;
		curr->time_start = &new->time_start;
		//curr->
		i++;
	}
	return (new);
}

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac < 5 || ac > 6)
		return (1);
	table = ft_init_table(ac, av);
	if (!table)
		return (1);
	return (0);
}
