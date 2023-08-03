/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:15:28 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 15:27:29 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	set_status_to_limbo(t_data *data)
{
	int i;

	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		pthread_mutex_lock(&(data->mutex_lock));
		data->philos[i++].status.state = LIMBO;
		pthread_mutex_unlock(&(data->mutex_lock));
	}
}

static bool	curr_time_more_than_next_meal_time(t_philo *philo)
{
	int64_t	current_time;

	current_time = get_current_time();
	pthread_mutex_lock(&(philo->data->mutex_lock));
	if (current_time > philo->status.next_meal_time)
	{
		pthread_mutex_unlock(&(philo->data->mutex_lock));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->mutex_lock));
	return (false);
}

static bool	philo_meals_less_than_num_meals(t_philo *philo)
{
	t_in_data	*in_data;

	in_data = &philo->data->in_data;
	pthread_mutex_lock(&(philo->data->mutex_lock));
	if (philo->status.meals < in_data->number_of_meals)
	{
		pthread_mutex_unlock(&(philo->data->mutex_lock));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->mutex_lock));
	return (false);
}

bool	life_monitor(t_data *data)
{
	int64_t	current_time;
	int		i;

	current_time = get_current_time();
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		if (curr_time_more_than_next_meal_time(&(data->philos[i])))
		{
			print_log(&data->philos[i], "DIED");
			set_status_to_limbo(data);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	meals_condition(t_data *data)
{
	t_philo	*philo;
	int		i;

	if (data->in_data.number_of_meals > 0)
	{
		i = 0;
		while (i < data->in_data.number_of_philosophers)
		{
			philo = &data->philos[i];
			if (philo_meals_less_than_num_meals(philo) == true)
			{
				return (true);
			}
			i++;
		}
	}
	else
		return (true);
	set_status_to_limbo(data);
	data->meals_condition = true;
	return (false);
}
