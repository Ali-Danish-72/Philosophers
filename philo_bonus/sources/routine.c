/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/23 19:49:50 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/23 17:27:10 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers_bonus.h"

void	ft_usleep(long int clock)
{
	long int	timer_counter;
	t_time		initial_timer;
	t_time		loop_timer;

	timer_counter = 0;
	gettimeofday(&initial_timer, NULL);
	while (timer_counter < (clock + 1))
	{
		gettimeofday(&loop_timer, NULL);
		timer_counter = (loop_timer.tv_sec - \
			initial_timer.tv_sec) * 1000;
		timer_counter += (loop_timer.tv_usec - \
			initial_timer.tv_usec) / 1000;
	}
}

long int	calculate_timestamp(t_philo *philo, bool for_printing)
{
	long int	ms;

	if (for_printing)
	{
		ms = (philo->current_time.tv_usec - philo->initial_time.tv_usec) / 1000;
		ms += (philo->current_time.tv_sec - philo->initial_time.tv_sec) * 1000;
	}
	else
	{
		ms = (philo->current_time.tv_sec - philo->start_time.tv_sec) * 1000;
		ms += (philo->current_time.tv_usec - philo->start_time.tv_usec) / 1000;
	}
	return (ms);
}

void	print_logs(t_philo *philo, int log_type)
{
	gettimeofday(&philo->current_time, NULL);
	sem_wait(philo->print_semaphore);
	if (log_type == 1)
		printf(F_LOG, calculate_timestamp(philo, true), philo->philo_count \
		- philo->process_count);
	else if (log_type == 2)
		printf(E_LOG, calculate_timestamp(philo, true), philo->philo_count \
		- philo->process_count);
	else if (log_type == 3)
		printf(S_LOG, calculate_timestamp(philo, true), philo->philo_count \
		- philo->process_count);
	else if (log_type == 4)
		printf(T_LOG, calculate_timestamp(philo, true), philo->philo_count \
		- philo->process_count);
	else if (log_type == 5)
		printf(D_LOG, calculate_timestamp(philo, true), philo->philo_count \
		- philo->process_count);
	sem_post(philo->print_semaphore);
}

void	check_death(t_philo *philo)
{
	sem_wait(philo->death_semaphore);
	usleep(5);
	sem_post(philo->death_semaphore);
	gettimeofday(&philo->current_time, NULL);
	if (calculate_timestamp(philo, false) >= philo->death_clock)
	{
		sem_wait(philo->death_semaphore);
		print_logs(philo, 5);
		exit(0);
	}
}

void	routine(t_philo *philo)
{
	gettimeofday(&philo->start_time, NULL);
	while (philo->meal_count--)
	{
		check_death(philo);
		sem_wait(philo->forks_semaphore);
		print_logs(philo, 1);
		sem_wait(philo->forks_semaphore);
		print_logs(philo, 1);
		check_death(philo);
		print_logs(philo, 2);
		gettimeofday(&philo->start_time, NULL);
		ft_usleep(philo->eat_clock);
		sem_post(philo->forks_semaphore);
		sem_post(philo->forks_semaphore);
		if (!philo->meal_count)
			break ;
		check_death(philo);
		print_logs(philo, 3);
		ft_usleep(philo->sleep_clock);
		check_death(philo);
		print_logs(philo, 4);
	}
	exit(0);
}
