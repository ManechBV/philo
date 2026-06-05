/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 07:03:02 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/05 11:40:55 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac < 5 || ac > 6)
		return (-1);
	table = ft_create_table(ac, av);
	if (!table)
		return (-1);
	if (ft_init_mutexes(table) != 0)
		return (-1);
	pthread_mutex_lock(&table->death_mut);
	table->should_die = 0;
	pthread_mutex_unlock(&table->death_mut);
	if (ft_init_philos(table) != 0)
		return (-1);
	pthread_join(table->monitor, NULL);
	ft_free_table(table);
	return (0);
}
