/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 14:11:55 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/23 17:29:07 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

/*				Library Inclusions				*/
# include <fcntl.h>
# include <semaphore.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <pthread.h>
# include <unistd.h>

/*				Macro Definitions				*/
# define INVALID_ARG_COUNT "Incorrect number of arguments. Use this format:\n"
# define ARGS1 "<number_of_philosophers> <time_to_die> <time_to_eat> "
# define ARGS2 "<time_to_sleep> [number_of_times_each_philosopher_must_eat]\n"
# define NEGATIVE_MESSAGE "Negative parameters detected. "
# define NEGATIVE_ARGS "Ensure no negative numbers are in the arguments.\n"
# define INVALID_ARGS "Non-numeric parameters detected. "
# define INVALID_MESSAGE "Ensure only numeric arguments are provided.\n"
# define NO_MEALS "The philosophers need to eat.\n"
# define D_SEM "death_semaphore"
# define D_SEM_FAIL "Opening of the death semaphore failed.\n"
# define F_SEM "forks_semaphore"
# define F_SEM_FAIL "Opening of the forks semaphore failed."
# define P_SEM "print_semaphore"
# define P_SEM_FAIL "Opening of the print semaphore failed.\n"
# define PHILO_PID_FAIL "Malloc of the philosophers' pids failed.\n"
# define PHILO_CHILD_FAIL "Fork of the philosophers' child failed.\n"
# define PHILO_DEATH_FAIL "Threading of the philosophers' death failed.\n"
# define F_LOG "%ld %d has taken a fork\n"
# define E_LOG "%ld %d is eating\n"
# define S_LOG "%ld %d is sleeping\n"
# define T_LOG "%ld %d is thinking\n"
# define D_LOG "%ld %d died\n"

/*				Type Definitions				*/
typedef struct timeval	t_time;
typedef struct s_philo	t_philo;

/*				Structs							*/
struct s_philo
{
	bool		end_simulation;
	int			status;
	pid_t		philosophers[200];
	pthread_t	death_threads[200];
	sem_t		*death_semaphore;
	sem_t		*forks_semaphore;
	sem_t		*print_semaphore;
	t_time		current_time;
	t_time		initial_time;
	t_time		start_time;
	uint		death_clock;
	uint		eat_clock;
	uint		meal_count;
	uint		philo_count;
	uint		process_count;
	uint		sleep_clock;
};

/*				Function Declarations			*/
long int		calculate_timestamp(t_philo *philo, bool for_printing);
void			check_death(t_philo *philo);
void			create_philosophers(t_philo *philo);
void			*death_thread(void *philosopher);
size_t			ft_atoi(char const *string);
void			ft_usleep(long int clock);
void			initialise_values(t_philo *philo, char **av);
int				one_philo(t_philo *philo);
void			parse(char **av);
void			print_error_message_and_exit(int status);
void			print_logs(t_philo *philo, int log_type);
void			routine(t_philo *philo);

//	Error Codes:
//	0 = Program completed successfully.
//	1 = Invalid number of arguments.
//	2 = Negative numbers in the arguments detected.
//	3 = Non-numeric arguments detected.
//	4 = Opening of the death semaphore failed.
//	5 = Opening of the forks semaphore failed.
//	6 = Opening of the print semaphore failed.
//	7 = Malloc of the philosophers' pids failed.
//	8 = Forking of the philosphers' processes failed.
//	9 = Threading of the philosophers' death failed.

#endif