/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:58:35 by mabenois          #+#    #+#             */
/*   Updated: 2026/05/03 02:00:41 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_take_forks(t_philo *curr)
{
	if (curr->id % 2 == 0)
	{
		pthread_mutex_lock(curr->fork_l);
		philo_print(curr, "\e[0;35mhas taken a fork");
		pthread_mutex_lock(curr->fork_r);
		philo_print(curr, "\e[1;35mhas taken a fork");
	}
	else
	{
		pthread_mutex_lock(curr->fork_r);
		philo_print(curr, "\e[0;35mhas taken a fork");
		pthread_mutex_lock(curr->fork_l);
		philo_print(curr, "\e[1;35mhas taken a fork");
	}
}

void	philo_release_forks(t_philo *curr)
{
		pthread_mutex_unlock(curr->fork_l);
		pthread_mutex_unlock(curr->fork_r);
}
