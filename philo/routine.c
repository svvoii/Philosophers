/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/01 20:01:54 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	if (philo->status.state != LIMBO)
		philo->status.state = EATING;
	else
	{
		pthread_mutex_unlock(&(philo->data->forks[philo->right_fork]));
		pthread_mutex_unlock(&(philo->data->forks[philo->left_fork]));
	}
}

void	eating(t_philo *philo)
{
	print_log(philo, "is eating");
	philo->status.meals++;
	philo->status.last_meal_time = get_current_time();
	philo->status.next_meal_time = philo->status.last_meal_time + \
									philo->data->in_data.time_to_die;
	ft_usleep(philo, philo->data->in_data.time_to_eat);
	if (philo->status.state != LIMBO)
		philo->status.state = SLEEPING;
	pthread_mutex_unlock(&(philo->data->forks[philo->left_fork]));
	pthread_mutex_unlock(&(philo->data->forks[philo->right_fork]));
}

void	sleeping(t_philo *philo)
{
	print_log(philo, "is sleeping");
	ft_usleep(philo, philo->data->in_data.time_to_sleep);
	if (philo->status.state != LIMBO)
		philo->status.state = THINKING;
}

void	thinking(t_philo *philo)
{
	print_log(philo, "is thinking");
	if (philo->status.state != LIMBO)
		philo->status.state = HUNGRY;
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;

	philo = (t_philo *)philo_ptr;
	while (philo->status.state != LIMBO)
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
