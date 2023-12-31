/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:07:19 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 17:22:22 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000) + (tp.tv_usec / 1000));
}

int64_t	timestamp(t_data *data)
{
	int64_t	current_time;

	current_time = get_current_time();
	return (current_time - data->start_time);
}

void	ft_usleep(t_philo *philo, int ms)
{
	int64_t	time;

	time = get_current_time();
	while (get_current_time() < time + ms)
	{
		if (state_limbo(philo) == true)
			break ;
		usleep(3);
	}
}

void	print_meals_condition(t_data *data)
{
	if (data->meals_condition)
	{
		pthread_mutex_lock(&(data->mutex_print_log));
		printf("%ld Everyone had enough meals\n", timestamp(data));
		pthread_mutex_unlock(&(data->mutex_print_log));
	}
}

void	print_log(t_philo *philo, char *message)
{
	t_data	*data;

	data = philo->data;
	if (state_limbo(philo) == false)
	{
		pthread_mutex_lock(&(philo->data->mutex_print_log));
		printf("%ld %d %s\n", timestamp(data), philo->id + 1, message);
		pthread_mutex_unlock(&(philo->data->mutex_print_log));
	}
}
