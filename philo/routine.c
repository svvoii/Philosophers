/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 17:36:27 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

bool	state_limbo(t_philo *philo)
{
	pthread_mutex_lock(&(philo->data->mutex_lock));
	if (philo->status.state == LIMBO)
	{
		pthread_mutex_unlock(&(philo->data->mutex_lock));
		return (true);
	}
	pthread_mutex_unlock(&(philo->data->mutex_lock));
	return (false);
}

bool	last_thread(t_philo *philo)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(data->mutex_start));
	if (data->start == true)
	{
		pthread_mutex_unlock(&(data->mutex_start));
		return (true);
	}
	pthread_mutex_unlock(&(data->mutex_start));
	return (false);
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	while (last_thread(philo) == false)
		usleep(10);
	while (state_limbo(philo) == false)
	{
		if (philo->status.state == HUNGRY)
		{
			pickup_forks(philo);
		}
		if (philo->status.state == EATING)
		{
			eating(philo);
		}
		if (philo->status.state == SLEEPING)
		{
			sleeping(philo);
		}
		if (philo->status.state == THINKING)
		{
			thinking(philo);
		}
	}
	return (NULL);
}
