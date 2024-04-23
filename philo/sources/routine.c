/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:49:50 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/22 17:59:36 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

void	pick_up_fork(t_philo *philo, t_consts *consts, bool mine)
{
	if (mine && !philo->has_left_fork)
	{
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_id - 1);
		if (!*(consts->forks + philo->fork_id - 1))
		{
			philo->has_left_fork = true;
			*(consts->forks + philo->fork_id - 1) = true;
		}
		pthread_mutex_unlock(consts->forks_mutexes + philo->fork_id - 1);
		if (philo->has_left_fork)
			print_logs(philo, 1);
	}
	else if (!mine && !philo->has_right_fork)
	{
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_next_id - 1);
		if (!*(consts->forks + philo->fork_next_id - 1))
		{
			philo->has_right_fork = true;
			*(consts->forks + philo->fork_next_id - 1) = true;
		}
		pthread_mutex_unlock(consts->forks_mutexes + philo->fork_next_id - 1);
		if (philo->has_right_fork)
			print_logs(philo, 1);
	}
}

void	drop_fork(t_philo *philo, t_consts *consts, bool mine)
{
	if (mine)
	{
		pthread_mutex_lock(consts->forks_mutexes + philo->fork_id - 1);
		philo->has_left_fork = false;
		*(consts->forks + philo->fork_id - 1) = false;
		pthread_mutex_unlock(consts->forks_mutexes + philo->fork_id - 1);
		return ;
	}
	pthread_mutex_lock(consts->forks_mutexes + philo->fork_next_id - 1);
	philo->has_right_fork = false;
	*(consts->forks + philo->fork_next_id - 1) = false;
	pthread_mutex_unlock(consts->forks_mutexes + philo->fork_next_id - 1);
}

int	eating(t_philo *philo, t_consts *consts)
{
	if (check_death(philo))
		return (1);
	pick_up_fork(philo, consts, true);
	if (philo->has_left_fork)
		pick_up_fork(philo, consts, false);
	if (!(philo->has_left_fork && philo->has_right_fork))
		print_logs(philo, 4);
	while (!(philo->has_right_fork && philo->has_left_fork))
	{
		if (check_death(philo))
			return (1);
		pick_up_fork(philo, consts, true);
		if (philo->has_left_fork)
			pick_up_fork(philo, consts, false);
	}
	print_logs(philo, 2);
	gettimeofday(&philo->start_time, NULL);
	ft_usleep(philo, philo->eat_clock);
	drop_fork(philo, consts, true);
	drop_fork(philo, consts, false);
	return (0);
}

void	*death_thread(void *consts)
{
	bool		death_flag;
	bool		end_simulation;
	t_consts	*constants;
	uint		index;

	constants = (t_consts *)consts;
	index = constants->philo_count;
	death_flag = false;
	end_simulation = false;
	while (!death_flag && !end_simulation)
	{
		pthread_mutex_lock(&(constants->philo + --index)->death_mutex);
		if ((constants->philo + index)->is_dead)
			death_flag = true;
		else if ((constants->philo + index)->end_simulation)
			end_simulation = true;
		pthread_mutex_unlock(&(constants->philo + index)->death_mutex);
		if (!index)
			index = constants->philo_count;
	}
	kill_the_philos(constants, death_flag);
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
		if (eating(philo, philo->constants))
			return (NULL);
		if (philo->meal_count && check_death(philo))
			return (NULL);
		if (philo->meal_count)
		{
			print_logs(philo, 3);
			ft_usleep(philo, philo->sleep_clock);
		}
	}
	pthread_mutex_lock(&philo->death_mutex);
	philo->end_simulation = true;
	pthread_mutex_unlock(&philo->death_mutex);
	return (NULL);
}
