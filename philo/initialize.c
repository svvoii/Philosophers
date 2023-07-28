/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:17:36 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/28 18:39:23 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_philo_struct(t_data *data)
{
	t_philo	*philo;
	long	total_philo;
	int		i;

	data->start_time = get_current_time();
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		/* initializing struct for each philosopher */
		philo = &(data->philos[i]);
		total_philo = data->in_data.number_of_philosophers;
		philo->id = i;
		philo->left_fork = i;
		philo->right_fork = (i + 1) % total_philo;
		philo->status.state = HUNGRY;
		philo->status.meals = 0;
		philo->status.last_meal_time = data->start_time;
		philo->status.next_meal_time = data->start_time + data->in_data.time_to_die;
		philo->data = data;
		i++;
	}
}

void	init_input_data(t_data *data)
{
	data->in_data.number_of_philosophers = PHILO_NUM;
	data->in_data.time_to_die = TIME_TO_DIE;
	data->in_data.time_to_eat = TIME_TO_EAT;
	data->in_data.time_to_sleep = TIME_TO_SLEEP;
	data->in_data.number_of_meals = MIN_MEALS;
}

bool	initialize_mutex(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&(data->mutex_print_log), NULL) != 0)
		return (false);
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL) != 0)
			return (false);
		i++;
	}
	return (true);
}

bool	launched_threads(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		philo = &(data->philos[i]);
		if (pthread_create(&(philo->thread), NULL, &routine, philo) != 0)
			return (false);
		i++;
	}
	/* might need to create additional thread for monitoring whether philos are alive */
	return (true);
}
