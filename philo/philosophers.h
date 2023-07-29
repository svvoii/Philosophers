/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:20:39 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/29 21:00:16 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdint.h>
# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <stdbool.h>
# include <sys/time.h>
# include <time.h>

# define	PHILO_NUM 		5
# define	TIME_TO_DIE 	800
# define	TIME_TO_EAT 	200
# define	TIME_TO_SLEEP 	200
# define	MIN_MEALS 		7

/*
enum	e_state
{
	NOT_ALIVE,
	HUNGRY,
	EATING,
	SLEEPING,
	THINKING
};
*/
enum	e_state
{
	NOT_ALIVE,
	FULL,
	ALIVE
};

/* this holds the initial input data */
typedef struct s_in_data
{
	long	number_of_philosophers;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
}	t_in_data;

/* this data reflects the current status of the philo */
typedef struct s_status
{
	enum e_state	state;
	int				meals;
	int64_t			last_meal_time;
	int64_t			next_meal_time;
}	t_status;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				left_fork;
	int				right_fork;
	t_status		status;
	struct s_data	*data;
}	t_philo;

typedef struct s_data
{
	int64_t			start_time;
	t_in_data		in_data;
	t_philo			philos[PHILO_NUM];
	pthread_mutex_t	forks[PHILO_NUM];
	pthread_mutex_t	mutex_print_log;
}	t_data;

/* main.c */

/* helpers.c */
long	get_current_time();
int64_t	timestamp(t_data *data);
void	print_log(t_philo *philo, char *message);
void	ft_usleep(int ms);

/* initialize.c */
void	init_philo_struct(t_data *data);
void	init_input_data(t_data *data);
bool	initialize_mutex(t_data *data);
bool	launched_threads(t_data *data);

/* monitor.c */
//bool	philosophers_alive(t_philo *philo);
bool	meals_condition(t_data *philo);
//bool	available_meals(t_philo *philo);
bool	life_monitor(t_data *data);

/* routine.c */
bool	pickup_forks(t_philo *philo);
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	*routine(void *philo_ptr);

/* DEBUG */
char	*print_state(enum e_state state);
void	print_philo(t_data *data);

/* ***** */

#endif