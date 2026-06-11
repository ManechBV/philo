/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/03 01:58:35 by mabenois          #+#    #+#             */
/*   Updated: 2026/06/12 00:49:17 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_take_fork(t_fork *fork)
{
		pthread_mutex_lock(&(fork->taken_mut));
		fork->taken = 1;
		pthread_mutex_unlock(&(fork->taken_mut));
		pthread_mutex_lock(&(fork->mut));
}

void	ft_drop_fork(t_fork *fork)
{
		pthread_mutex_unlock(&(fork->mut));
		pthread_mutex_lock(&(fork->taken_mut));
		fork->taken = 0;
		pthread_mutex_unlock(&(fork->taken_mut));
}

int		ft_fork_is_taken(t_fork *fork)
{
	pthread_mutex_lock(&(fork->taken_mut));
	if (fork->taken == 1)
	{
		pthread_mutex_unlock(&(fork->taken_mut));
		return (1);
	}
	pthread_mutex_unlock(&(fork->taken_mut));
	return (0);
}

void	philo_take_forks(t_philo *curr)
{
	t_fork	*first;
	t_fork	*second;

	first = curr->fork_r;
	second = curr->fork_l;
	while (1)
	{
		/*
		if (ft_fork_is_taken(first) == 1 || ft_fork_is_taken(second) == 1)
			continue ;
			*/
		if (first == second)
		{
			ft_take_fork(first);
			philo_print(curr, "\e[0;93mhas taken a fork", 0);
			return ;
		}
		/*
		if (curr->is_last == 1 && (curr->id % 2 != 0))
			if (ft_fork_is_taken(first) == 1)
				continue ;
				*/
		ft_take_fork(first);
		/*
		if (curr->is_last == 1 && (curr->id % 2 != 0))
		{
			if (ft_fork_is_taken(second) == 1)
			{
				ft_drop_fork(first);
				continue ;
			}
		}
				*/
		philo_print(curr, "\e[0;93mhas taken a fork", 0);
		ft_take_fork(second);
		philo_print(curr, "\e[1;93mhas taken a fork", 0);
		/*
		if (ft_fork_is_taken(second) == 1)
		{
			ft_drop_fork(first);
			continue;
		}
		*/
		break;
	}
}

void	philo_release_forks(t_philo *curr)
{
	ft_drop_fork(curr->fork_l);
	if (curr->fork_l != curr->fork_r)
		ft_drop_fork(curr->fork_r);
}
