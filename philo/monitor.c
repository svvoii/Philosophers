/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:15:28 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/31 16:37:53 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	life_monitor(t_data *data)
{
	int64_t	current_time;
	int		i;

	current_time = get_current_time();
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		if (current_time > data->philos[i].status.next_meal_time)
		{
			/* DEBUG 
			pthread_mutex_lock(&(data->mutex_print_log));
			printf("\t\t[%d] next_meal[%ld] - time[%ld]\n", data->philos[i].id, data->philos[i].status.next_meal_time, get_current_time());
			pthread_mutex_unlock(&(data->mutex_print_log));
			*/
			/* ***** */
			print_log(&data->philos[i], "has DIED");
			i = 0;
			while (i < data->in_data.number_of_philosophers)
				data->philos[i++].status.state = LIMBO;
			return (false);
		}
		i++;
	}
	return (true);
}

bool	meals_target(t_philo *philo, int number_of_meals)
{
	return (philo->status.meals < number_of_meals);
}

bool	meals_condition(t_data *data)
{
	t_philo	*philo;
	int		i;

	if (data->in_data.number_of_meals > 0)
	{
		/*
		pthread_mutex_lock(&(data->mutex_print_log));
		printf("\tmeals_condition\n");
		pthread_mutex_unlock(&(data->mutex_print_log));
		*/
		i = 0;
		while (i < data->in_data.number_of_philosophers)
		{
			philo = &data->philos[i];
			if (philo->status.meals < data->in_data.number_of_meals)
			{
				return (true);
			}
			i++;
		}
	}
	else
		return (true);
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		data->philos[i++].status.state = LIMBO;	
	/*
		pthread_mutex_lock(&(data->mutex_print_log));
		printf("\t[%d] state: [%s]\n", i - 1, print_state(data->philos[0].status.state));
		pthread_mutex_unlock(&(data->mutex_print_log));
	*/
	}
	return (false);
}
