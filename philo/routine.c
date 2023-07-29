/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:12:52 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/29 18:25:58 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
bool	pickup_forks(t_philo *philo)
{
    t_data	*data = philo->data;
	int		locked;

	if (philo->id % 2)
	{
		locked = pthread_mutex_lock(&(data->forks[philo->left_fork]));
		if (locked == 0)
		{
			printf("\tphilo:[%d] locked:[%d] left_fork: [%d]\n",philo->id, locked, philo->left_fork);
			locked = pthread_mutex_lock(&(data->forks[philo->right_fork]));
			if (locked == 0)
			{
				printf("\tphilo:[%d] locked:[%d] right_fork: [%d]\n", philo->id, locked, philo->right_fork);
				return (true);
			}
			else
			{
				printf("\tphilo:[%d] unlocking left_fork: [%d]\n", philo->id, philo->left_fork);
				pthread_mutex_unlock(&(data->forks[philo->left_fork]));
			}
		}
	}
	else
	{
		locked = pthread_mutex_lock(&(data->forks[philo->right_fork]));
		if (locked == 0)
		{
			printf("\tphilo:[%d] locked:[%d] right_fork: [%d]\n", philo->id, locked, philo->right_fork);
			locked = pthread_mutex_lock(&(data->forks[philo->left_fork]));
			if (locked == 0)
			{
				printf("\tphilo:[%d] locked:[%d] left_fork: [%d]\n", philo->id, locked, philo->left_fork);
				return (true);
			}
			else
			{
				printf("\tphilo:[%d] unlocking right_fork: [%d]\n", philo->id, philo->right_fork);
				pthread_mutex_unlock(&(data->forks[philo->right_fork]));
			}
		}
	}
	return (false);
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
		{
			//printf("\tphilo:[%d] locked left_fork: [%d]\n",philo->id, philo->left_fork);
			if (pthread_mutex_lock(&(data->forks[philo->right_fork])) == 0)
			{
				//printf("\tphilo:[%d] locked right_fork: [%d]\n", philo->id, philo->right_fork);
				return (true);
			}
		}
	}
	else
	{
		if (pthread_mutex_lock(&(data->forks[philo->right_fork])) == 0)
		{
			//printf("\tphilo:[%d] locked right_fork: [%d]\n", philo->id, philo->right_fork);
			if (pthread_mutex_lock(&(data->forks[philo->left_fork])) == 0)
			{
				//printf("\tphilo:[%d] locked left_fork: [%d]\n", philo->id, philo->left_fork);
				return (true);
			}
		}
	}
	return (false);
}

void	eating(t_philo *philo)
{
    t_data *data = philo->data;

	print_log(philo, "is EATING");
    philo->status.meals++;
    philo->status.last_meal_time = get_current_time();
    philo->status.next_meal_time = philo->status.last_meal_time + data->in_data.time_to_die;

	/* DEBUG */
	pthread_mutex_lock(&(philo->data->mutex_print_log));
	printf("\t\t\tnext_meal - time[%ld]:[%ld]\n", get_current_time(), philo->status.next_meal_time - get_current_time());

    ft_usleep(data->in_data.time_to_eat);

	printf("\t\t\tnext_meal - time[%ld]:[%ld]\n", get_current_time(), philo->status.next_meal_time - get_current_time());
	pthread_mutex_unlock(&(philo->data->mutex_print_log));
	/* ***** */

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

void	sleeping_and_thinking(t_philo *philo)
{
    t_data *data = philo->data;

	print_log(philo, "is SLIPING");
	/* DEBUG */
	pthread_mutex_lock(&(philo->data->mutex_print_log));
	printf("\t\t\tnext_meal - time[%ld]:[%ld]\n", get_current_time(), philo->status.next_meal_time - get_current_time());

    ft_usleep(data->in_data.time_to_sleep);

	printf("\t\t\tnext_meal - time[%ld]:[%ld]\n", get_current_time(), philo->status.next_meal_time - get_current_time());
	pthread_mutex_unlock(&(philo->data->mutex_print_log));
	/* ***** */
	print_log(philo, "is THINKING");
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;
	long	total_philo;

	philo = (t_philo *)philo_ptr;
	total_philo = philo->data->in_data.number_of_philosophers;

	//while (meals_condition(philo))
	while (meals_condition(philo))
	{
		if (philosophers_alive(philo))
		{
			if (pickup_forks(philo))
				eating(philo);
		}
		else
			break ;
		if (philosophers_alive(philo))
			sleeping_and_thinking(philo);
		else
			break ;
	}
	return (NULL);
}

/*
void	*routine(void *philo_ptr)
{
	t_philo	*philo;
	long	total_philo;

	philo = (t_philo *)philo_ptr;
	total_philo = philo->data->in_data.number_of_philosophers;

	while (philosophers_alive(philo) && meals_condition(philo))
	{
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
*/
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