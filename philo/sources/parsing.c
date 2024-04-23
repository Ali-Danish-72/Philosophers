/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:24:04 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/22 20:01:11 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

int	one_philo(t_consts *consts)
{
	t_time		start;
	t_time		end;
	long int	milli_seconds;

	if (!consts->philo_count || !consts->meal_count)
		return (0);
	milli_seconds = 0;
	printf("0 1 is thinking\n");
	gettimeofday(&start, NULL);
	while (milli_seconds < (consts->death_clock + 1))
	{
		gettimeofday(&end, NULL);
		milli_seconds = (end.tv_sec - start.tv_sec) * 1000;
		milli_seconds += (end.tv_usec - start.tv_usec) / 1000;
	}
	printf("%ld 1 died\n", milli_seconds);
	return (0);
}

int	create_threads(t_consts *consts)
{
	consts->thread_count = consts->philo_count;
	gettimeofday(&consts->initial_time, NULL);
	while (consts->thread_count--)
	{
		if (pthread_create(&(consts->philo + consts->thread_count)->thread, \
			NULL, routine, consts->philo + consts->thread_count))
			return (consts->exit_code = 10);
		usleep(100);
	}
	if (pthread_create(&consts->death_thread, NULL, death_thread, consts))
		return (consts->exit_code = 11);
	pthread_join(consts->death_thread, NULL);
	consts->thread_count = consts->philo_count;
	while (consts->thread_count--)
		pthread_join((consts->philo + consts->thread_count)->thread, NULL);
	return (consts->exit_code = 0);
}

int	initialise_philosophers(t_consts *consts)
{
	uint	*count;

	consts->mutex_count = consts->philo_count;
	count = &consts->mutex_count;
	while ((*count)--)
	{
		if (pthread_mutex_init(consts->forks_mutexes + *count, NULL))
			return (consts->exit_code = 8);
		if (pthread_mutex_init(&(consts->philo + *count)->death_mutex, NULL))
			return (consts->exit_code = 9);
		(consts->philo + *count)->fork_id = consts->philo_count - *count;
		(consts->philo + *count)->fork_next_id = consts->philo_count - \
			*count + 1;
		(consts->philo + *count)->death_clock = consts->death_clock;
		(consts->philo + *count)->eat_clock = consts->eat_clock;
		(consts->philo + *count)->sleep_clock = consts->sleep_clock;
		(consts->philo + *count)->meal_count = consts->meal_count;
		(consts->philo + *count)->constants = consts;
	}
	return (0);
}

int	initialise_constants(t_consts *consts, char **av)
{
	consts->philo_count = ft_atoi(*av++);
	consts->death_clock = ft_atoi(*av++);
	consts->eat_clock = ft_atoi(*av++);
	consts->sleep_clock = ft_atoi(*av++);
	consts->meal_count = ft_atoi(*av);
	if (consts->philo_count == 1 || !consts->philo_count || !consts->meal_count)
		return (one_philo(consts));
	if (pthread_mutex_init(&consts->print_mutex, NULL))
		return (consts->exit_code = 4);
	consts->philo = malloc(consts->philo_count * sizeof(t_philo));
	if (!consts->philo)
		return (consts->exit_code = 5);
	consts->forks = malloc(consts->philo_count * sizeof(bool));
	if (!consts->forks)
		return (consts->exit_code = 6);
	consts->forks_mutexes = malloc(consts->philo_count * sizeof(t_mutex));
	if (!consts->forks_mutexes)
		return (consts->exit_code = 7);
	memset(consts->philo, 0, consts->philo_count * sizeof(t_philo));
	memset(consts->forks, 0, consts->philo_count * sizeof(bool));
	memset(consts->forks_mutexes, 0, consts->philo_count * sizeof(t_mutex));
	return (0);
}

int	parse(t_consts *consts, char **av)
{
	int	i;
	int	j;

	i = -1;
	while (*(av + ++i))
	{
		if (**(av + i) == '-')
			return (consts->exit_code = 2);
		j = -1;
		while (*(*(av + i) + ++j))
		{
			if (*(*(av + i) + j) < 48 || *(*(av + i) + j) > 57)
				return (consts->exit_code = 3);
		}
	}
	consts->philo = NULL;
	consts->forks = NULL;
	consts->forks_mutexes = NULL;
	if (initialise_constants(consts, av) || \
		(consts->philo_count > 1 && consts->meal_count && \
		initialise_philosophers(consts)))
		return (-1);
	return (0);
}
