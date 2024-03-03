/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 17:33:12 by mdanish           #+#    #+#             */
/*   Updated: 2024/03/03 15:55:13 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	mutex_destruction(t_constants *consts)
{
	pthread_mutex_destroy(&consts->time_mutex);
	if (consts->exit_code > 5 || !consts->exit_code)
		pthread_mutex_destroy(&consts->print_mutex);
	if (consts->exit_code == 9 || consts->exit_code == 10)
	{
		if (consts->exit_code == 10)
			pthread_mutex_destroy(consts->forks_mutexes + \
			consts->mutex_count++);
		while (consts->mutex_count != consts->philo_count)
		{
			pthread_mutex_destroy(consts->forks_mutexes + consts->mutex_count);
			pthread_mutex_destroy(&(consts->philo + \
				consts->mutex_count++)->death_mutex);
		}
	}
	else if (consts->exit_code > 10 || !consts->exit_code)
	{
		while (consts->philo_count--)
		{
			pthread_mutex_destroy(consts->forks_mutexes + consts->philo_count);
			pthread_mutex_destroy(&(consts->philo + \
				consts->philo_count)->death_mutex);
		}
	}
}

int	memory_management(t_constants *consts)
{
	if (consts->exit_code > 4 || \
		(!consts->exit_code && consts->philo_count != 1))
	{
		mutex_destruction(consts);
		while (consts->exit_code == 11 && ++consts->thread_count != \
			consts->philo_count)
			pthread_join((consts->philo + consts->thread_count)->thread, NULL);
		while (consts->exit_code == 12 && consts->philo_count--)
			pthread_join((consts->philo + consts->philo_count)->thread, NULL);
		free(consts->philo);
		free(consts->forks);
		free(consts->forks_mutexes);
	}
	return (consts->exit_code);
}

int	print_error_message(t_constants *consts)
{
	if (consts->exit_code == 1)
		printf("%s%s%s", INVALID_ARG_COUNT, ARGS1, ARGS2);
	else if (consts->exit_code == 2)
		printf("%s%s", NEGATIVE_MESSAGE, NEGATIVE_ARGS);
	else if (consts->exit_code == 3)
		printf("%s%s", INVALID_ARGS, INVALID_MESSAGE);
	else if (consts->exit_code == 4)
		printf("%s", TIME_MUTEX_FAIL);
	else if (consts->exit_code == 5)
		printf("%s", PRINT_MUTEX_FAIL);
	else if (consts->exit_code == 6)
		printf("%s", PHILO_FAIL);
	else if (consts->exit_code == 7)
		printf("%s", FORKS_FAIL);
	else if (consts->exit_code == 8)
		printf("%s", FORKS_MUTEXES_FAIL);
	else if (consts->exit_code == 9)
		printf("%s", FORK_MUTEX_FAIL);
	else if (consts->exit_code == 10)
		printf("%s", DEATH_MUTEX_FAIL);
	else if (consts->exit_code == 11)
		printf("%s", PHILO_THREAD_FAIL);
	else if (consts->exit_code == 12)
		printf("%s", DEATH_THREAD_FAIL);
	return (memory_management(consts));
}

void	kill_the_philos(t_constants *constants, bool death_flag)
{
	unsigned int	end_simulation_count;
	unsigned int	index;

	index = constants->philo_count;
	while (death_flag && index--)
	{
		pthread_mutex_lock(&(constants->philo + index)->death_mutex);
		(constants->philo + index)->end_simulation = true;
		pthread_mutex_unlock(&(constants->philo + index)->death_mutex);
	}
	end_simulation_count = false;
	while (!death_flag && index--)
	{
		pthread_mutex_lock(&(constants->philo + index)->death_mutex);
		if ((constants->philo + index)->end_simulation)
			end_simulation_count += true;
		pthread_mutex_unlock(&(constants->philo + index)->death_mutex);
		if (!index && end_simulation_count != constants->philo_count)
		{
			end_simulation_count = false;
			index = constants->philo_count;
		}
	}
}

int	main(int ac, char **av)
{
	t_constants	consts;

	if (ac < 5 || ac > 6)
		return (consts.exit_code = 1, print_error_message(&consts));
	if (parse(&consts, ++av))
		return (print_error_message(&consts));
	if (consts.philo_count == 1 || !consts.philo_count || !consts.meal_count)
		return (0);
	if (create_threads(&consts))
		return (print_error_message(&consts));
	return (print_error_message(&consts));
}
