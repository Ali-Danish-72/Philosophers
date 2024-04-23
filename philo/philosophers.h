/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdanish <mdanish@student.42abudhabi.ae>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 18:15:15 by mdanish           #+#    #+#             */
/*   Updated: 2024/04/23 09:04:38 by mdanish          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/*				Library Inclusions				*/
# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

/*				Message Definitions				*/
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

/*				Type Definitions				*/
typedef pthread_mutex_t	t_mutex;
typedef struct timeval	t_time;
typedef struct s_consts	t_consts;
typedef struct s_philo	t_philo;

/*				Structs							*/
struct s_philo
{
	bool		has_left_fork;
	bool		has_right_fork;
	bool		end_simulation;
	bool		is_dead;
	pthread_t	thread;
	t_mutex		death_mutex;
	t_time		start_time;
	t_time		current_time;
	t_time		initial_timer_time;
	t_time		loop_timer_time;
	t_consts	*constants;
	uint		death_clock;
	uint		eat_clock;
	uint		fork_id;
	uint		fork_next_id;
	uint		sleep_clock;
	uint		meal_count;
};

struct s_consts
{
	bool		*forks;
	int			exit_code;
	pthread_t	death_thread;
	t_mutex		print_mutex;
	t_mutex		*forks_mutexes;
	t_mutex		time_mutex;
	t_philo		*philo;
	t_time		initial_time;
	uint		philo_count;
	uint		mutex_count;
	uint		thread_count;
	uint		death_clock;
	uint		eat_clock;
	uint		sleep_clock;
	uint		meal_count;
};

/*				Function Declarations			*/
long int		calculate_timestamp(t_philo *philo, bool for_printing);
void			*death_thread(void *consts);
void			drop_fork(t_philo *philo, t_consts *consts, bool mine);
int				eating(t_philo *philo, t_consts *consts);
size_t			ft_atoi(char const *string);
void			ft_usleep(t_philo *philo, long int clock);
int				initialise_constants(t_consts *constants, char **av);
int				initialise_philosophers(t_consts *consts);
int				check_death(t_philo *philo);
int				create_threads(t_consts *philo);
void			kill_the_philos(t_consts *constants, bool death_flag);
int				memory_management(t_consts *consts);
void			mutex_destruction(t_consts *consts);
int				one_philo(t_consts *consts);
int				parse(t_consts *philo, char **av);
void			pick_up_fork(t_philo *philo, t_consts *consts, bool me);
int				print_error_message(t_consts *philo);
void			print_logs(t_philo *philo, int log_type);
void			*routine(void *philosopher);

//	Error Codes:
//	0 = Program completed successfully.
//	1 = Invalid number of arguments.
//	2 = Negative numbers in the arguments detected.
//	3 = Non-numeric arguments detected.
//	4 = Initialising of the print mutex failed.
//	5 = Malloc of the philosophers failed.
//	6 = Malloc of the forks failed.
//	7 = Malloc of the fork mutexes failed.
//	8 = Initialising of the fork mutexes failed.
//	9 = Initialising of the death mutexes failed.
//	10 = Creating of the philosopher threads failed.
//	11 = Creating of the death thread failed.

#endif