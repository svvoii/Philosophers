/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:15:28 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/29 21:00:33 by sbocanci         ###   ########.fr       */
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
		/* DEBUG  
		pthread_mutex_lock(&(data->mutex_print_log));
		printf("\t\t\t[%d] next_meal - time[%ld]\n", data->philos[i].id, data->philos[i].status.next_meal_time - get_current_time());
		pthread_mutex_unlock(&(data->mutex_print_log));
		*/
		/* ***** */
		if (current_time >= data->philos[i].status.next_meal_time)
		{
			print_log(&data->philos[i], "is DEAD");
			i = -1;
			while (++i < data->in_data.number_of_philosophers)
				data->philos[i].status.state = NOT_ALIVE;
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
			if (philo->status.meals < data->in_data.number_of_meals)
				return (true);
			i++;
		}
	}
	else
		return (true);
	i = -1;
	while (++i < data->in_data.number_of_philosophers)
		data->philos[i].status.state = FULL;	
	return (false);
}
