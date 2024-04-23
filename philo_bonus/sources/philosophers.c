/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:03:11 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/23 17:23:51 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philosophers_bonus.h"

void	print_error_message_and_exit(int status)
{
	if (status == 1)
		printf("%s%s%s", INVALID_ARG_COUNT, ARGS1, ARGS2);
	else if (status == 2)
		printf("%s%s", NEGATIVE_MESSAGE, NEGATIVE_ARGS);
	else if (status == 3)
		printf("%s%s", INVALID_ARGS, INVALID_MESSAGE);
	else if (status == 4)
		printf("%s", D_SEM_FAIL);
	else if (status == 5)
		printf("%s", F_SEM_FAIL);
	else if (status == 6)
		printf("%s", P_SEM_FAIL);
	else if (status == 7)
		printf("%s", PHILO_PID_FAIL);
	else if (status == 8)
		printf("%s", PHILO_CHILD_FAIL);
	else if (status == 9)
		printf("%s", PHILO_DEATH_FAIL);
	exit(status);
}

void	*death_thread(void *philosopher)
{
	int		pid;
	int		code;
	uint	index;
	t_philo	*philo;

	philo = (t_philo *)philosopher;
	pid = *(philo->philosophers + philo->process_count);
	waitpid(pid, &code, 0);
	index = 0;
	while (index < philo->philo_count)
		kill(*(philo->philosophers + index++), SIGKILL);
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	philo;

	if (ac < 5 || ac > 6)
		print_error_message_and_exit(1);
	parse(++av);
	initialise_values(&philo, av);
	create_philosophers(&philo);
	while (++philo.process_count < philo.philo_count)
		pthread_join(*(philo.death_threads + philo.process_count), NULL);
	exit(0);
}
