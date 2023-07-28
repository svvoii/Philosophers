/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/28 18:53:04 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
bool	pickup_forks(t_philo *philo)
{
    t_data	*data = philo->data;
	int		attempt;

    //pthread_mutex_lock(&(data->forks[philo->left_fork]));
    //pthread_mutex_lock(&(data->forks[philo->right_fork]));

	attempt = pthread_mutex_trylock(&(data->forks[philo->left_fork]));
	if (attempt != 0)
	{
		print_log(philo, "attempted to pick up LEFT fork but its locked..");
		return (false);
	}
	else
		print_log(philo, "picked up LEFT fork");
	attempt = pthread_mutex_trylock(&(data->forks[philo->right_fork]));
	if (attempt != 0)
	{
		print_log(philo, "attempted to pick up RIGHT fork but its locked..");
		pthread_mutex_unlock(&(data->forks[philo->left_fork]));
		return (false);
	}
	else
		print_log(philo, "picked up RIGHT fork");
	return (true);
}
*/

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

void	eating(t_philo *philo)
{
    t_data *data = philo->data;

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

    ft_usleep(data->in_data.time_to_sleep);
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;
	long	total_philo;

	philo = (t_philo *)philo_ptr;
	total_philo = philo->data->in_data.number_of_philosophers;

	while (philosophers_alive(philo) && available_meals(philo))
	{
		/*
		*/
		if (philo->status.state == HUNGRY)
		{
			//print_log(philo, "is HUNGRY");
			if (pickup_forks(philo))
				philo->status.state = EATING;
		}
		if (philo->status.state == EATING)
		{
			print_log(philo, "is EATING");
			eating(philo);
			philo->status.state = SLEEPING;
		}
		if (philo->status.state == SLEEPING)
		{
			print_log(philo, "is SLIPING");
			sleeping(philo);
			philo->status.state = THINKING;
		}
		if (philo->status.state == THINKING)
		{
			print_log(philo, "is THINKING");
			philo->status.state = HUNGRY;
		}
	}
	return (NULL);
}

	/*
	while (1)
	{
		if (!ft_eat())
			break ;
		if (!ft_sleep())
			break ;
		if (!ft_think())
			break ;
	}
	return (NULL);
	*/