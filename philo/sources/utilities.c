/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utilities.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 17:23:17 by mdanish           #+#    #+#             */
/*   Updated: 2024/03/02 19:56:02 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers.h"

size_t	ft_atoi(char const *string)
{
	int		sign;
	size_t	number;

	if (!string)
		return (-1);
	number = 0;
	sign = 1;
	while (*string == 32 || (*string > 8 && *string < 14))
		string++;
	if (*string == '+')
		string++;
	else if (*string == '-' && string++)
		sign = -1;
	while (*string == 48)
		string++;
	while (*string > 47 && *string < 58)
		number = (number * 10) + *string++ - 48;
	return (number * sign);
}

long int	calculate_timestamp(t_philo *philo, bool for_printing)
{
	long int	ms;

	if (for_printing)
	{
		ms = (philo->current_time.tv_sec - \
			philo->constants->initial_time.tv_sec) * 1000;
		ms += (philo->current_time.tv_usec - \
			philo->constants->initial_time.tv_usec) / 1000;
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
	if (log_type == 3)
		philo->print_thinking = false;
	gettimeofday(&philo->current_time, NULL);
	pthread_mutex_lock(&philo->constants->print_mutex);
	if (log_type == 1)
	{
		printf(FORK_LOG, calculate_timestamp(philo, true), philo->fork_id);
		printf(FORK_LOG, calculate_timestamp(philo, true), philo->fork_id);
		printf(EAT_LOG, calculate_timestamp(philo, true), philo->fork_id);
	}
	else if (log_type == 2)
		printf(SLEEP_LOG, calculate_timestamp(philo, true), philo->fork_id);
	else if (log_type == 3)
		printf(THINK_LOG, calculate_timestamp(philo, true), philo->fork_id);
	else if (log_type == 4)
		printf(DEATH_LOG, calculate_timestamp(philo, true), philo->fork_id);
	pthread_mutex_unlock(&philo->constants->print_mutex);
}

int	check_death(t_philo *philo)
{
	bool	is_dead;

	is_dead = false;
	pthread_mutex_lock(&philo->death_mutex);
	if (philo->end_simulation)
		is_dead = true;
	pthread_mutex_unlock(&philo->death_mutex);
	if (is_dead)
		return (1);
	if (calculate_timestamp(philo, false) >= philo->death_clock)
	{
		pthread_mutex_lock(&philo->death_mutex);
		philo->is_dead = true;
		pthread_mutex_unlock(&philo->death_mutex);
		print_logs(philo, 4);
		return (1);
	}
	return (0);
}
