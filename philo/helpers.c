/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:07:19 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/31 16:38:24 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

long	get_current_time()
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

void	ft_usleep(int ms)
{
	int64_t	time;

	time = get_current_time();
	usleep(ms * 920);
	while (get_current_time() < time + ms)
	{
		usleep(ms * 3);
	}
}

void	print_log(t_philo *philo, char *message)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(philo->data->mutex_print_log));
	printf("%ld %d %s\n", timestamp(data), philo->id, message);
	//printf("(meals:[%d])\n", philo->status.meals);
	//printf("next_meal:[%ld])\n", philo->status.next_meal_time - current_time);
	pthread_mutex_unlock(&(philo->data->mutex_print_log));
}

/*
void	print_log(t_philo *philo, char *message)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(philo->data->mutex_print_log));
	printf("[%ld] Philo [%d] %s\n", timestamp(data), philo->id, message);
	pthread_mutex_unlock(&(philo->data->mutex_print_log));
}
*/