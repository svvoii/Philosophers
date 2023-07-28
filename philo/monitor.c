/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:15:28 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/28 19:00:35 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/* This will monitor whether all philos are still alive, inside routine */
bool	philosophers_alive(t_philo *philo)
{
	t_data	*data;
	t_philo	*philo_tmp;
	int64_t	current_time;
	int		i;

	data = philo->data;
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		philo_tmp = &data->philos[i];
		current_time = get_current_time();
		/* DEBUG */
		//printf("\tnext_meal - cur_time:[%ld]\tPhilo [%d] status: [%s]\n", philo->status.next_meal_time - current_time, philo_tmp->id, print_state(philo->status.state));
		/* ***** */
		if (current_time > philo_tmp->status.next_meal_time)
		{
			print_log(philo_tmp, "is no longer alive :(");
			return (false);
		}
		i++;
	}
	return (true);
}

bool	available_meals(t_philo *philo)
{
	t_data	*data;
	t_philo	*philo_tmp;
	int		i;

	data = philo->data;
	if (data->in_data.number_of_meals > 0)
	{
		i = 0;
		while (i < data->in_data.number_of_philosophers)
		{
			philo_tmp = &data->philos[i];
			if (philo_tmp->status.meals >= data->in_data.number_of_meals)
				return (false);
			i++;
		}
	}
	return (true);
}

void	*life_monitor(void *data_ptr)
{
	printf("\tlife_monitor\tdata @ [%p]\n", data_ptr);
	return (NULL);
}
