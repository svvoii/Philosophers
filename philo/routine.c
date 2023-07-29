/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/29 20:47:23 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*

bool	pickup_forks(t_philo *philo)
{
    t_data	*data = philo->data;

	if (philo->id % 2)
	{
		pthread_mutex_lock(&(data->forks[philo->left_fork]));
		pthread_mutex_lock(&(data->forks[philo->right_fork]));
	}
	else
	{
		pthread_mutex_lock(&(data->forks[philo->right_fork]));
		pthread_mutex_lock(&(data->forks[philo->left_fork]));
	}
	return (true);
}
*/

bool	pickup_forks(t_philo *philo)
{
    t_data	*data = philo->data;

	if (philo->left_fork == philo->right_fork)
		philo->status.state = NOT_ALIVE;
	else if (philo->id % 2)
	{
		if (pthread_mutex_lock(&(data->forks[philo->left_fork])) == 0)
			if (pthread_mutex_lock(&(data->forks[philo->right_fork])) == 0)
				return (true);
	}
	else
	{
		if (pthread_mutex_lock(&(data->forks[philo->right_fork])) == 0)
			if (pthread_mutex_lock(&(data->forks[philo->left_fork])) == 0)
				return (true);
	}
	return (false);
}

void	eating(t_philo *philo)
{
    t_data *data = philo->data;

	pickup_forks(philo);
	if (philo->status.state == ALIVE)
		print_log(philo, "is EATING");
    philo->status.meals++;
    philo->status.last_meal_time = get_current_time();
    philo->status.next_meal_time = philo->status.last_meal_time + data->in_data.time_to_die;

    ft_usleep(data->in_data.time_to_eat);
	if (philo->id % 2)
	{
		pthread_mutex_unlock(&(data->forks[philo->left_fork]));
		pthread_mutex_unlock(&(data->forks[philo->right_fork]));
	}
	else
	{
		pthread_mutex_unlock(&(data->forks[philo->right_fork]));
		pthread_mutex_unlock(&(data->forks[philo->left_fork]));
	}
}

void	sleeping(t_philo *philo)
{
    t_data *data = philo->data;

	print_log(philo, "is SLIPING");
    ft_usleep(data->in_data.time_to_sleep);
}

void	thinking(t_philo *philo)
{
	print_log(philo, "is THINKING");
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	while (philo->status.state == ALIVE)
	{
		if (philo->status.state == ALIVE)
			eating(philo);
		if (philo->status.state == ALIVE)
			sleeping(philo);
		if (philo->status.state == ALIVE)
			thinking(philo);
	}
	return (NULL);
}
