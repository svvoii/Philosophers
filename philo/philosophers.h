/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:20:39 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 17:38:00 by sbocanci         ###   ########.fr       */
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

# define MAX_PHILO 200	

enum	e_state
{
	LIMBO,
	HUNGRY,
	EATING,
	SLEEPING,
	THINKING
};

/* this holds the initial input data */
typedef struct s_in_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_meals;
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
	t_philo			philos[MAX_PHILO];
	pthread_mutex_t	forks[MAX_PHILO];
	pthread_mutex_t	mutex_print_log;
	pthread_mutex_t	mutex_lock;
	pthread_mutex_t	mutex_start;
	bool			start;
	bool			meals_condition;
}	t_data;

/* main.c */

/* helpers.c */
long	get_current_time(void);
int64_t	timestamp(t_data *data);
void	ft_usleep(t_philo *philo, int ms);
void	print_meals_condition(t_data *data);
void	print_log(t_philo *philo, char *message);

/* initialize.c */
void	init_philo_struct(t_data *data);
bool	initialize_mutex(t_data *data);
bool	launched_threads(t_data *data);

/* monitor.c */
bool	meals_condition(t_data *philo);
bool	life_monitor(t_data *data);

/* parsing_utils */
int		ft_strlen(char *str);
int		ft_strcmp(char *s1, char *s2);
int		ft_atoi(char *str);
bool	is_number(char *str);

/* parsing */
bool	valid_number(char *str);
bool	init_input_data(t_data *data, int argc, char **argv);
bool	parsing(t_data *data, int argc, char **argv);

/* routine_utils.c */
void	pickup_forks(t_philo *philo);
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo);

/* routine.c */
bool	state_limbo(t_philo *philo);
void	pickup_forks(t_philo *philo);
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	*routine(void *philo_ptr);

#endif