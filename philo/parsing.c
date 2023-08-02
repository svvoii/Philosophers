/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:17:36 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/02 18:04:34 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

/*
#define INT_MAX_STR	"2147483647"
*/

bool	valid_number(char *str)
{
	size_t	len;

	if (!is_number(str))
		return (false);
	len = ft_strlen(str);
	if (len > 10)
		return (false);
	else if (len == 10 && ft_strcmp("2147483647", str))
	{
		printf("Error: %s: argument must be number within MAX_INT\n", str);
		return (false);
	}
	return (true);
}

bool	init_input_data(t_data *data, int argc, char **argv)
{
	data->in_data.number_of_philosophers = ft_atoi(argv[1]);
	if (data->in_data.number_of_philosophers > MAX_PHILO)
	{
		printf("Error: Do not test with more than 200 philosophers.\n");
		return (false);
	}
	data->in_data.time_to_die = ft_atoi(argv[2]);
	data->in_data.time_to_eat = ft_atoi(argv[3]);
	data->in_data.time_to_sleep = ft_atoi(argv[4]);
	if (data->in_data.time_to_die < 60 || data->in_data.time_to_eat < 60
		|| data->in_data.time_to_sleep < 60)
	{
		printf("Error: 'time to (die/eat/sleep)' shall be more than 60ms.\n");
		return (false);
	}
	data->in_data.number_of_meals = 0;
	if (argc == 6)
		data->in_data.number_of_meals = ft_atoi(argv[5]);
	data->meals_condition = false;
	data->start = false;
	return (true);
}

bool	parsing(t_data *data, int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		printf("Error: wrong number of arguments\n");
		printf("Useage example:\n");
		printf("\t./Philosophers 5 800 200 200 7\n");
		printf("\t5:\t'Number of philosophers'\n");
		printf("\t800:\t'time to die'\n");
		printf("\t200:\t'time to eat'\n");
		printf("\t200:\t'time to sleep'\n");
		printf("\t7:\t'minimun meals' (optional)\n");
		return (false);
	}
	else
	{
		i = 0;
		while (argv[++i])
			if (!valid_number(argv[i]))
				return (false);
	}
	return (init_input_data(data, argc, argv));
}
