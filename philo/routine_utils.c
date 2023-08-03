/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 17:32:56 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static void	reset_meal_time(t_philo *philo)
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

void	eating(t_philo *philo)
{
	print_log(philo, "is eating");
	reset_meal_time(philo);
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
