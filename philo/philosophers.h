/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:15:15 by mdanish           #+#    #+#             */
/*   Updated: 2024/02/28 13:13:33 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define INVALID_ARG_COUNT "Incorrect number of arguments. Use this format:\n"
# define ARGS1 "<number_of_philosophers> <time_to_die> <time_to_eat> "
# define ARGS2 "<time_to_sleep> [number_of_times_each_philosopher_must_eat]\n"
# define NEGATIVE_MESSAGE "Negative parameters detected. "
# define NEGATIVE_ARGS "Ensure no negative numbers are in the arguments.\n"
# define INVALID_ARGS "Non-numeric parameters detected. "
# define INVALID_MESSAGE "Ensure only numeric arguments are provided.\n"
# define NO_MEALS "The philosophers need to eat.\n"
# define PRINT_MUTEX_FAIL "Initializing of the print mutex failed.\n"
# define PHILO_FAIL "Mallocing of the philosophers failed.\n"
# define FORKS_FAIL "Mallocing of the forks failed.\n"
# define FORKS_MUTEXES_FAIL "Mallocing of the fork mutexes failed.\n"
# define FORK_MUTEX_FAIL "Initializing of the fork mutexes failed.\n"
# define DEATH_MUTEX_FAIL "Initializing of the death mutex failed.\n"
# define TIME_MUTEX_FAIL "Initializing of the time mutex failed.\n"
# define PHILO_THREAD_FAIL "Creating of the philosopher threads failed.\n"
# define DEATH_THREAD_FAIL "Creating of the death thread failed.\n"
# define FORK_LOG "%ld %d has taken a fork\n"
# define EAT_LOG "%ld %d is eating\n"
# define SLEEP_LOG "%ld %d is sleeping\n"
# define THINK_LOG "%ld %d is thinking\n"
# define DEATH_LOG "%ld %d died\n"

typedef pthread_mutex_t	t_mutex;
typedef struct timeval	t_time;
typedef struct s_consts	t_constants;
typedef struct s_philo	t_philo;

struct s_philo
{
	bool			can_eat;
	bool			end_simulation;
	bool			is_dead;
	bool			print_thinking;
	int				death_type;
	pthread_t		thread;
	t_mutex			death_mutex;
	t_time			start_time;
	t_time			current_time;
	t_constants		*constants;
	unsigned int	death_clock;
	unsigned int	eat_clock;
	unsigned int	fork_id;
	unsigned int	fork_next_id;
	unsigned int	sleep_clock;
	unsigned int	meal_count;
};

struct s_consts
{
	bool			*forks;
	int				exit_code;
	pthread_t		death_thread;
	t_mutex			print_mutex;
	t_mutex			*forks_mutexes;
	t_mutex			time_mutex;
	t_philo			*philo;
	t_time			initial_time;
	unsigned int	philo_count;
	unsigned int	mutex_count;
	unsigned int	thread_count;
	unsigned int	death_clock;
	unsigned int	eat_clock;
	unsigned int	sleep_clock;
	unsigned int	meal_count;
};

long int			calculate_timestamp(t_philo *philo, bool for_printing);
void				*death_thread(void *consts);
size_t				ft_atoi(char const *string);
int					initialise_constants(t_constants *constants, char **av);
int					initialise_philosophers(t_constants *consts);
int					check_death(t_philo *philo);
int					create_threads(t_constants *philo);
int					one_philo(t_constants *consts);
int					parse(t_constants *philo, char **av);
int					print_error_message(t_constants *philo);
void				print_logs(t_philo *philo, int log_type);
void				*routine(void *philosopher);

//	Error Codes:
//	0 = Program completed successfully.
//	1 = Invalid number of arguments.
//	2 = Negative numbers in the arguments detected.
//	3 = Non-numeric arguments detected.
//	4 = No meals alloted to the philosophers.
//	5 = Initialising of the print mutex failed.
//	6 = Malloc of the philosophers failed.
//	7 = Malloc of the forks failed.
//	8 = Malloc of the fork mutexes failed.
//	9 = Initialising of the fork mutexes failed.
//	10 = Initialising of the death mutexes failed.
//	11 = Creating of the philosopher threads failed.
//	12 = Creating of the death thread failed.

#endif