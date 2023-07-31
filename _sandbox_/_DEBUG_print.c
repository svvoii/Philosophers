/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _DEBUG_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:07:19 by sbocanci          #+#    #+#             */
/*   Updated: 2023/07/31 15:05:26 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../philo/philosophers.h"

/* DEBUG */
/*
char	*print_state(enum e_state state)
{
	if (state == ALIVE)
		return ("ALIVE");
	return ("NOT_ALIVE");
}
*/
char	*print_state(enum e_state state)
{
	if (state == HUNGRY)
		return ("HUNGRY");
	else if (state == EATING)
		return ("EATING");
	else if (state == EATING)
		return ("FULL");
	else if (state == SLEEPING)
		return ("SLEEPING");
	else if (state == THINKING)
		return ("THINKING");
	return ("IN LIMBO");
}

void	print_philo(t_data *data)
{
	t_philo	*philo;
	int		i = 0;

	printf("..print_philo..\n");
	while (i < data->in_data.number_of_philosophers)
	{
		philo = &(data->philos[i]);
		printf("\t[%d] philo id: [%d]\t", i, philo->id);
		printf("left_fork: [%d], right_fork: [%d]\t", philo->left_fork, philo->right_fork);
		printf("state: [%s]\t", print_state(philo->status.state));
		printf("meals:[%d]\n", philo->status.meals);
		printf("\t\tlast_meal_time:[%ld]ms\n", philo->status.last_meal_time);
		printf("\t\tnext_meal_time:[%ld]ms\n", philo->status.next_meal_time);
		printf("\t\tnext - last:[%ld]ms\n", philo->status.next_meal_time - philo->status.last_meal_time);
		i++;
	}
}

	/* DEBUG */
	//print_philo(data);
	/* ***** */
/* ***** */
