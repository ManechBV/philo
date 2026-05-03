/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 07:03:02 by mabenois          #+#    #+#             */
/*   Updated: 2026/05/03 03:14:09 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac < 5)
		return (-1);
	table = ft_create_table(av);
	if (!table)
		return (-1);
	if (ft_init_mutexes(table) != 0)
		return (-1);
	pthread_mutex_lock(&table->death_mut);
	table->should_die = 0;
	pthread_mutex_unlock(&table->death_mut);
	if (ft_init_philos(table) != 0)
		return (-1);
	return (0);
}
