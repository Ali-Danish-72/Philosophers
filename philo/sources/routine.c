/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:49:50 by mdanish           #+#    #+#             */
/*   Updated: 2024/02/29 12:18:19 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	what_to_do_with_forks(bool pick_up, t_philo *philo, t_constants *consts)
{
	if (philo->fork_id == consts->philo_count)
	{
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_next_id - 1);
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_id - 1);
	}
	else
	{
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_id - 1);
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_next_id - 1);
	}
	if (pick_up && !*(consts->forks + philo->fork_id - 1) && \
			!*(consts->forks + philo->fork_next_id - 1))
	{
		philo->can_eat = true;
		*(consts->forks + philo->fork_id - 1) = true;
		*(consts->forks + philo->fork_next_id - 1) = true;
		gettimeofday(&philo->start_time, NULL);
	}
	else if (!pick_up)
	{
		*(consts->forks + philo->fork_id - 1) = false;
		*(consts->forks + philo->fork_next_id - 1) = false;
	}
	pthread_mutex_unlock(consts->forks_mutexes + philo->fork_id - 1);
	pthread_mutex_unlock(consts->forks_mutexes + philo->fork_next_id - 1);
}

int	eating(t_philo *philo, t_constants *consts)
{
	int	timer_counter;

	while (!philo->can_eat && !check_death(philo))
	{
		what_to_do_with_forks(true, philo, consts);
		if (!philo->can_eat && philo->print_thinking && !check_death(philo))
			print_logs(philo, 3);
		else if (philo->can_eat)
		{
			print_logs(philo, 1);
		}
	}
	timer_counter = 10;
	while (timer_counter--)
	{
		if (check_death(philo))
			return (print_logs(philo, 4), 1);
		usleep(philo->eat_clock * 100);
	}
	what_to_do_with_forks(false, philo, consts);
	return (0);
}

int	sleeping(t_philo *philo)
{
	int	timer_counter;

	if (check_death(philo))
		return (print_logs(philo, 4), 1);
	print_logs(philo, 2);
	timer_counter = 10;
	while (timer_counter--)
	{
		if (check_death(philo))
			return (print_logs(philo, 4), 1);
		usleep(philo->sleep_clock * 100);
	}
	return (0);
}

void	*death_thread(void *consts)
{
	bool			death_flag;
	t_constants		*constants;
	unsigned int	index;
	unsigned int	end_simulation_count;

	constants = (t_constants *)consts;
	index = constants->philo_count;
	death_flag = false;
	end_simulation_count = false;
	while (!death_flag && !end_simulation_count)
	{
		pthread_mutex_lock(&(constants->philo + --index)->death_mutex);
		if ((constants->philo + index)->is_dead)
			death_flag = true;
		if ((constants->philo + index)->end_simulation)
			end_simulation_count = true;
		pthread_mutex_unlock(&(constants->philo + index)->death_mutex);
		if (!index)
			index = constants->philo_count;
	}
	index = constants->philo_count;
	while (death_flag && index--)
	{
		pthread_mutex_lock(&(constants->philo + index)->death_mutex);
		(constants->philo + index)->end_simulation = true;
		pthread_mutex_unlock(&(constants->philo + index)->death_mutex);
	}
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
	return (NULL);
}

void	*routine(void *philosopher)
{
	t_philo		*philo;

	philo = (t_philo *)philosopher;
	if (philo->fork_next_id == philo->constants->philo_count + 1)
		philo->fork_next_id = 1;
	gettimeofday(&philo->start_time, NULL);
	while (philo->meal_count--)
	{
		philo->print_thinking = true;
		philo->can_eat = false;
		if (eating(philo, philo->constants))
			return (NULL);
		if (philo->meal_count && sleeping(philo))
			return (NULL);
	}
	pthread_mutex_lock(&philo->death_mutex);
	philo->end_simulation = true;
	pthread_mutex_unlock(&philo->death_mutex);
	return (NULL);
}
