/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sbocanci <sbocanci@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 12:17:36 by sbocanci          #+#    #+#             */
/*   Updated: 2023/08/01 19:56:57 by sbocanci         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

int	ft_strcmp(char *s1, char *s2)
{
	unsigned int	i;

	i = 0;
	while (s1[i] == s2[i] && (s1[i] != '\0' || s2[i] != '\0'))
		i++;
	return (s1[i] - s2[i]);
}

int	ft_atoi(char *str)
{
	unsigned int	sign;
	unsigned int	res;

	while ((*str >= 9 && *str <= 13) || (*str == 32))
		str++;
	sign = 1;
	res = 0;
	if (*str == '-')
	{
		sign *= -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		res *= 10;
		res += (*str - '0');
		str++;
	}
	return (res * sign);
}

bool	is_number(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (i > 10)
		{
			printf("Error: %s: argument must be number within MAX_INT\n", str);
			return (false);
		}
		if (str[i] < '0' || str[i] > '9')
		{
			if (str[0] == '-')
				printf("Error: %s: argument cannot be negative\n", str);
			else
				printf("Error: %s: argument must be a number\n", str);
			return (false);
		}
		i++;
	}
	return (true);
}
