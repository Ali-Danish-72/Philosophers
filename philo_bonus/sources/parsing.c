/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 20:04:56 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/23 17:10:36 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers_bonus.h"

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

int	one_philo(t_philo *philo)
{
	long int	ms;
	int			timer;

	if (philo->philo_count && philo->meal_count)
	{
		timer = 10;
		printf("0 1 is thinking\n");
		gettimeofday(&philo->initial_time, NULL);
		while (timer--)
			usleep(philo->death_clock * 100);
		gettimeofday(&philo->current_time, NULL);
		ms = (philo->current_time.tv_usec - philo->initial_time.tv_usec) / 1000;
		ms += (philo->current_time.tv_sec - philo->initial_time.tv_sec) * 1000;
		printf("%ld 1 died\n", ms);
	}
	exit(0);
}

void	create_philosophers(t_philo *philo)
{
	while (philo->process_count--)
	{
		*(philo->philosophers + philo->process_count) = fork();
		if (*(philo->philosophers + philo->process_count) < 0)
			philo->status = 8;
		if (philo->status)
			break ;
		if (!*(philo->philosophers + philo->process_count))
			routine(philo);
		if (pthread_create(philo->death_threads + philo->process_count, NULL, \
			death_thread, philo))
			philo->status = 9;
		if (philo->status)
			break ;
		usleep(200);
	}
	if (philo->status == 9)
		kill(*(philo->philosophers + philo->process_count), SIGKILL);
	while (philo->status && ++philo->process_count < philo->philo_count)
	{
		kill(*(philo->philosophers + philo->process_count), SIGKILL);
		pthread_join(*(philo->death_threads + philo->process_count), NULL);
	}
	if (philo->status)
		print_error_message_and_exit(philo->status);
}

void	initialise_values(t_philo *philo, char **av)
{
	philo->philo_count = ft_atoi(*av++) % 200;
	philo->death_clock = ft_atoi(*av++);
	philo->eat_clock = ft_atoi(*av++);
	philo->sleep_clock = ft_atoi(*av++);
	philo->meal_count = ft_atoi(*av);
	philo->process_count = philo->philo_count;
	philo->end_simulation = false;
	philo->status = 0;
	if (philo->philo_count == 1 || !philo->philo_count || !philo->meal_count)
		one_philo(philo);
	sem_unlink(D_SEM);
	philo->death_semaphore = sem_open(D_SEM, O_CREAT, 666, 1);
	if (!philo->death_semaphore)
		print_error_message_and_exit(4);
	sem_unlink(F_SEM);
	philo->forks_semaphore = sem_open(F_SEM, O_CREAT, 666, philo->philo_count);
	if (!philo->forks_semaphore)
		print_error_message_and_exit(5);
	sem_unlink(P_SEM);
	philo->print_semaphore = sem_open(P_SEM, O_CREAT, 666, 1);
	if (!philo->print_semaphore)
		print_error_message_and_exit(6);
	gettimeofday(&philo->initial_time, NULL);
}

void	parse(char **av)
{
	int	i;
	int	j;

	i = -1;
	while (*(av + ++i))
	{
		if (**(av + i) == '-')
			print_error_message_and_exit(2);
		j = -1;
		while (*(*(av + i) + ++j))
		{
			if (*(*(av + i) + j) < 48 || *(*(av + i) + j) > 57)
				print_error_message_and_exit(3);
		}
	}
}
