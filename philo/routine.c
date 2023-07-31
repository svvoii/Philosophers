/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/31 14:31:16by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
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
*/

void	pickup_forks(t_philo *philo)
{
    t_data	*data = philo->data;

	if (philo->id % 2)
	{
		pthread_mutex_lock(&(data->forks[philo->left_fork]));
		print_log(philo, "picked LEFT");
		pthread_mutex_lock(&(data->forks[philo->right_fork]));
		print_log(philo, "picked RIGHT");
	}
	else
	{
		pthread_mutex_lock(&(data->forks[philo->right_fork]));
		print_log(philo, "picked RIGHT");
		pthread_mutex_lock(&(data->forks[philo->left_fork]));
		print_log(philo, "picked LEFT");
	}
	if (philo->status.state != LIMBO)
		philo->status.state = EATING;
	else
	{
		pthread_mutex_unlock(&(data->forks[philo->right_fork]));
		pthread_mutex_unlock(&(data->forks[philo->left_fork]));
	}
}

void	eating(t_philo *philo)
{
    t_data *data = philo->data;

	/*
	pickup_forks(philo);
	*/
    philo->status.meals++;
    philo->status.last_meal_time = get_current_time();
    philo->status.next_meal_time = philo->status.last_meal_time + data->in_data.time_to_die;

	if (philo->status.state != LIMBO)
		print_log(philo, "is eating");
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
	if (philo->status.state != LIMBO)
		philo->status.state = SLEEPING;
}

void	sleeping(t_philo *philo)
{
    t_data *data = philo->data;

	print_log(philo, "is sleeping");
    ft_usleep(data->in_data.time_to_sleep);
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
		if (philo->status.state == LIMBO)
			break ;
	}
	/*
	*/
		pthread_mutex_lock(&(philo->data->mutex_print_log));
		printf("\t..routine.. [%d] state: [%s]\n", philo->id, print_state(philo->status.state));
		pthread_mutex_unlock(&(philo->data->mutex_print_log));
	return (NULL);
}
/*
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
*/