/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:17:36 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/02 18:26:07 by sbocanci         ###   ########.fr       */
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
		philo = &(data->philos[i]);
		total_philo = data->in_data.number_of_philosophers;
		philo->id = i;
		philo->left_fork = i;
		philo->right_fork = (i + 1) % total_philo;
		philo->status.state = HUNGRY;
		philo->status.meals = 0;
		philo->status.last_meal_time = data->start_time;
		philo->status.next_meal_time = data->start_time + \
									data->in_data.time_to_die;
		philo->data = data;
		i++;
	}
}

bool	initialize_mutex(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&(data->mutex_print_log), NULL) != 0)
		return (false);
	if (pthread_mutex_init(&(data->mutex_lock), NULL) != 0)
		return (false);
	if (pthread_mutex_init(&(data->mutex_start), NULL) != 0)
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

void	ft_start(t_data *data)
{
	t_philo	*philo;
	int		i;

	data->start_time = get_current_time();
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		philo = &(data->philos[i]);
		philo->status.last_meal_time = data->start_time;
		philo->status.next_meal_time = data->start_time + \
									data->in_data.time_to_die;
		i++;
	}
	pthread_mutex_lock(&(data->mutex_start));
	data->start = true;
	pthread_mutex_unlock(&(data->mutex_start));
}

bool	launched_threads(t_data *data)
{
	int		i;

	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		if (pthread_create(&(data->philos[i].thread), NULL, \
							&routine, &(data->philos[i])) != 0)
			return (false);
		i++;
	}
	ft_start(data);
	return (true);
}
