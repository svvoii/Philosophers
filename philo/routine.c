/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 15:58:45 by sbocanci         ###   ########.fr       */
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

void	pickup_forks(t_philo *philo)
{
	if (philo->left_fork == philo->right_fork)
		return ;
	if (philo->id % 2)
	{
		pthread_mutex_lock(&(philo->data->forks[philo->left_fork]));
		print_log(philo, "picked LEFT fork");
		pthread_mutex_lock(&(philo->data->forks[philo->right_fork]));
		print_log(philo, "picked RIGHT fork");
	}
	else
	{
		pthread_mutex_lock(&(philo->data->forks[philo->right_fork]));
		print_log(philo, "picked RIGHT fork");
		pthread_mutex_lock(&(philo->data->forks[philo->left_fork]));
		print_log(philo, "picked LEFT fork");
	}
	if (state_limbo(philo) == false)
		philo->status.state = EATING;
	else
	{
		pthread_mutex_unlock(&(philo->data->forks[philo->right_fork]));
		pthread_mutex_unlock(&(philo->data->forks[philo->left_fork]));
	}
}

void	reset_meal_time(t_philo *philo)
{
	t_status	*status;
	int			time_to_die;

	status = &philo->status;
	time_to_die = philo->data->in_data.time_to_die;
	status->last_meal_time = get_current_time();
	pthread_mutex_lock(&(philo->data->mutex_lock));
	status->meals++;
	status->next_meal_time = status->last_meal_time + time_to_die;
	pthread_mutex_unlock(&(philo->data->mutex_lock));
}

void	eating(t_philo *philo)
{
	print_log(philo, "is eating");
	reset_meal_time(philo);
	/*
	philo->status.meals++;
	philo->status.last_meal_time = get_current_time();
	philo->status.next_meal_time = philo->status.last_meal_time + \
									philo->data->in_data.time_to_die;
	*/
	ft_usleep(philo, philo->data->in_data.time_to_eat);
	if (state_limbo(philo) == false)
		philo->status.state = SLEEPING;
	pthread_mutex_unlock(&(philo->data->forks[philo->left_fork]));
	pthread_mutex_unlock(&(philo->data->forks[philo->right_fork]));
}

void	sleeping(t_philo *philo)
{
	print_log(philo, "is sleeping");
	ft_usleep(philo, philo->data->in_data.time_to_sleep);
	if (state_limbo(philo) == false)
		philo->status.state = THINKING;
}

void	thinking(t_philo *philo)
{
	print_log(philo, "is thinking");
	if (state_limbo(philo) == false)
		philo->status.state = HUNGRY;
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
	{
		usleep(10);
	}
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
