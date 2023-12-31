/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/01 19:29:19 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/03 17:17:39 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	destroy_mutex(t_data *data);

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	if (parsing(&data, argc, argv) == false)
		return (1);
	if (!initialize_mutex(&data))
		return (printf("Mutex init error\n"), 1);
	init_philo_struct(&data);
	if (!launched_threads(&data))
		return (printf("Thread Error\n"), 1);
	while (meals_condition(&data))
		if (!life_monitor(&data))
			break ;
	i = 0;
	while (i < data.in_data.number_of_philosophers)
		if (pthread_join(data.philos[i++].thread, NULL) != 0)
			return (printf("Pthread join error\n"), 1);
	print_meals_condition(&data);
	return (destroy_mutex(&data));
}

static int	destroy_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->in_data.number_of_philosophers)
		if (pthread_mutex_destroy(&(data->forks[i++])) != 0)
			return (printf("Pthread mutex destroy error\n"), 1);
	if (pthread_mutex_destroy(&(data->mutex_print_log)) != 0)
		return (printf("Pthread mutex destroy error\n"), 1);
	if (pthread_mutex_destroy(&(data->mutex_lock)) != 0)
		return (printf("Pthread mutex destroy error\n"), 1);
	if (pthread_mutex_destroy(&(data->mutex_start)) != 0)
		return (printf("Pthread mutex destroy error\n"), 1);
	return (0);
}
