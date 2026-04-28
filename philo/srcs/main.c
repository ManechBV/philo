/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabenois <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/08 15:56:57 by mabenois          #+#    #+#             */
/*   Updated: 2026/04/25 09:01:13 by mabenois         ###   ########.fr       */
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
	int	time_die;
	int	time_eat;
	int	time_sleep;
}	t_times;

typedef	struct	s_philo
{
	pthread_t		thread;
	int				id;
	suseconds_t		start_time;
	suseconds_t		time;
	suseconds_t		time_of_last_meal;
	suseconds_t		time_left;
	t_times			*times;
	pthread_mutex_t	*fork_r;
	pthread_mutex_t	*fork_l;
	int				*stop_simulation;
	pthread_mutex_t	*mutex_stop;
}	t_philo;

int	ft_atoi(char *s)
{
	int	i;
	int	ret;

	i = 0;
	ret = 0;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		ret = (ret * 10) + (s[i] - '0');
		i++;
	}
	return (ret);
}

void	get_philo_time(t_philo *curr)
{
	struct timeval	tmp;

	if (gettimeofday(&tmp, NULL) != 0)
		return ;
	if (curr->start_time == -1)
	{
		curr->start_time = (tmp.tv_sec * 1000) + (tmp.tv_usec / 1000);
		curr->time_of_last_meal = 0;
	}
	curr->time = ((tmp.tv_sec * 1000) + (tmp.tv_usec / 1000)) - curr->start_time;
	curr->time_left = (curr->times->time_die / 1000) - (curr->time - curr->time_of_last_meal);
}

void	philo_print(t_philo *self, char *str)
{
		get_philo_time(self);
		printf("\e[1;33m%lu\t\e[0;37m%d\t%s\n", self->time, self->id, str);
}

void	*philo_routine(void *curr)
{
	t_philo	*self;

	self = (t_philo*)curr;
	get_philo_time(self);
	//printf("%zu Philo %zu created with id %d\n", self->time, self->thread, self->id);
	while (1)
	{
		if (self->id % 2 == 0)
		{
			pthread_mutex_lock(self->fork_l);
			philo_print(self, "\e[0;36mhas taken a fork");
			if (self->time_left < 0)
			{
				pthread_mutex_unlock(self->fork_l);
				philo_print(self, "\e[0;31mfucking died");
				return (NULL);
			}
			pthread_mutex_lock(self->fork_r);
			philo_print(self, "\e[0;36mhas taken a fork");
			if (self->time_left < 0)
			{
				pthread_mutex_unlock(self->fork_l);
				pthread_mutex_unlock(self->fork_r);
				philo_print(self, "\e[0;31mfucking died");
				return (NULL);
			}
		}
		else
		{
			pthread_mutex_lock(self->fork_r);
			philo_print(self, "\e[0;36mhas taken a fork");
			if (self->time_left < 0)
			{
				pthread_mutex_unlock(self->fork_r);
				philo_print(self, "\e[0;31mfucking died");
				return (NULL);
			}
			pthread_mutex_lock(self->fork_l);
			philo_print(self, "\e[0;36mhas taken a fork");
			if (self->time_left < 0)
			{
				pthread_mutex_unlock(self->fork_r);
				pthread_mutex_unlock(self->fork_l);
				philo_print(self, "\e[0;31mfucking died");
				return (NULL);
			}
		}

		philo_print(self, "\e[0;32mis eating");
		self->time_of_last_meal = self->time;

		if (self->time_of_last_meal + self->times->time_die <= self->time + (self->times->time_eat / 1000))
		{
			usleep(self->time_left * 1000);
			pthread_mutex_unlock(self->fork_l);
			pthread_mutex_unlock(self->fork_r);
			philo_print(self, "\e[0;31mfucking died");
			return (NULL);
		}
		else
			usleep(self->times->time_eat);
		
		pthread_mutex_unlock(self->fork_l);
		pthread_mutex_unlock(self->fork_r);

		philo_print(self, "\e[0;34mis sleeping");

		if (self->time_of_last_meal + self->times->time_die <= self->time + (self->times->time_sleep / 1000))
		{
			usleep(self->time_left * 1000);
			get_philo_time(self);
			printf("self->time: %lu\tself->time_of_last_meal: %lu\ttime_to_die: %d\n", self->time, self->time_of_last_meal, self->times->time_die);
			philo_print(self, "\e[0;31mfucking died");
			return (NULL);
		}
		else
			usleep(self->times->time_sleep);

		philo_print(self, "\e[0;33mis thinking");
		if (self->time - self->time_of_last_meal >= self->times->time_die / 1000)
		{
			get_philo_time(self);
			printf("self->time: %lu\tself->time_of_last_meal: %lu\ttime_to_die: %d\n", self->time, self->time_of_last_meal, self->times->time_die);
			philo_print(self, "\e[0;31mfucking died");
			return (NULL);
		}
	}
	return (NULL);
}

t_philo	*create_philos(int nb_philo, pthread_mutex_t *forks, t_times *times)
{
	t_philo	*philos;
	int		i;

	philos = malloc(sizeof(t_philo) * (nb_philo + 1));
	if (!philos)
		return (NULL);
	i = 0;
	while (i < nb_philo)
	{
		philos[i].id = i + 1;
		philos[i].start_time = -1;
		int ok = i - 1;
		if (ok < 0)
			ok = nb_philo - 1;
		philos[i].fork_l = &forks[ok];
		philos[i].fork_r = &forks[i];
		printf("philo %d forkl %d forkr %d\n", i, ok, i);
		philos[i].times = times;
		if (pthread_create(&philos[i].thread, NULL, philo_routine, &philos[i]) != 0)
		{
			while (--i >= 0)
				pthread_join(philos[i].thread, NULL);
			free(philos);
			return (NULL);
		}
		i++;
	}
	philos[i].id = -1;
	return (philos);
}

void	free_philos(t_philo *philos)
{
	int	i;

	i = 0;
	while (philos[i].id != -1)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	free(philos);
}

pthread_mutex_t	*create_forks(int nb)
{
	pthread_mutex_t	*forks;

	forks = malloc(sizeof(pthread_mutex_t) * (nb + 1));
	if (!forks)
		return (NULL);
	while (nb > 0)
	{
		pthread_mutex_init(&forks[nb], NULL);
		nb--;
	}
	return (forks);
}

int	main(int ac, char **av)
{
	int				nb_philo;
	t_times			times;
	t_philo			*philos;
	pthread_mutex_t	*forks;

	if (ac < 5)
		return (-1);
	nb_philo = ft_atoi(av[1]);
	if (nb_philo == 0)
		return (-1);

	times.time_die = ft_atoi(av[2]) * 1000;
	times.time_eat = ft_atoi(av[3]) * 1000;
	times.time_sleep = ft_atoi(av[4]) * 1000;

	forks = create_forks(nb_philo);
	if (!forks)
		return (-1);

	philos = create_philos(nb_philo, forks, &times);
	if (!philos)
		return (-1);

	free_philos(philos);
	return (0);
}
