/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/25 07:03:02 by mabenois          #+#    #+#             */
/*   Updated: 2026/04/28 07:55:21 by mabenois         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>

typedef struct	s_times
{
	long long	die;
	long long	eat;
	long long	sleep;
}	t_times;

typedef struct	s_philo
{
	pthread_t		thread;
	int				id;
	pthread_mutex_t	*fork_l;
	pthread_mutex_t	*fork_r;
	t_times			*times;
	pthread_mutex_t	*start_mut;
	pthread_mutex_t	*print_mut;
	suseconds_t		time;
	suseconds_t		time_start;
}	t_philo;

typedef struct	s_table
{
	long long		nb_philos;
	t_philo			*philos;
	pthread_mutex_t	*forks;
	t_times			*times;
	pthread_mutex_t	start_mut;
	pthread_mutex_t	print_mut;
}	t_table;

long long	ft_atol(char *s)
{
	long long	i;
	long long	ret;

	i = 0;
	ret = 0;
	if (!s)
		return (-1);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (-1);
		ret = (ret * 10) + (s[i] - '0');
		i++;
	}
	return (ret);
}

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
}

void	ft_philo_wait_for_start(t_philo *self)
{
	struct timeval	time;

	pthread_mutex_lock(self->start_mut);
	pthread_mutex_unlock(self->start_mut);
	if (gettimeofday(&time, NULL) != 0)
		return ;
	return ;
}

void	philo_print(t_philo *curr, char *message)
{
	pthread_mutex_lock(curr->print_mut);
	printf("philo %d %s\n", curr->id, message);
	pthread_mutex_unlock(curr->print_mut);
}

void	philo_take_forks(t_philo *curr)
{
	if (curr->id % 2 == 0)
	{
		pthread_mutex_lock(curr->fork_l);
		philo_print(curr, "has taken a fork");
		pthread_mutex_lock(curr->fork_r);
		philo_print(curr, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(curr->fork_r);
		philo_print(curr, "has taken a fork");
		pthread_mutex_lock(curr->fork_l);
		philo_print(curr, "has taken a fork");
	}
}

void	philo_release_forks(t_philo *curr)
{
		pthread_mutex_unlock(curr->fork_l);
		pthread_mutex_unlock(curr->fork_r);
}

void	*philo_routine(void *curr)
{
	t_philo	*self;

	self = (t_philo*)curr;
	//philo_print(self, "created, waiting for start");
	ft_philo_wait_for_start(self);
	//philo_print(self, "started");
	while (1)
	{
		philo_take_forks(self);
		philo_print(self, "is eating");
		usleep(self->times->eat * 1000);
		philo_release_forks(self);
		philo_print(self, "is sleeping");
		usleep(self->times->sleep * 1000);
		philo_print(self, "is thinking");
	}
	return (NULL);
}

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
	return (0);
}

int	main(int ac, char **av)
{
	t_table	*table;

	if (ac < 5)
		return (-1);
	table = ft_create_table(av);
	if (!table)
		return (-1);
	// DEBUG
	printf("time_die: %lld\ntime_eat: %lld\ntime_sleep: %lld\n", table->times->die, table->times->eat, table->times->sleep);
	// END DEBUG
	if (ft_init_mutexes(table) != 0)
		return (-1);
	if (ft_init_philos(table) != 0)
		return (-1);
	// TODO faire une fonction free_philos
	for (long long i = 0; i < table->nb_philos; i++)
		pthread_join(table->philos[i].thread, NULL);
	//
	return (0);
}
