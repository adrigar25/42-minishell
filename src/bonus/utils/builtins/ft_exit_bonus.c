/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:46:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <limits.h>

int	ft_is_numeric(const char *str)
{
	int					i;
	long long			result;
	int					sign;
	unsigned long long	limit;

	if (!str || *str == '\0')
		return (0);
	i = 0;
	sign = 1;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (str[i] == '\0')
		return (0);
	result = 0;
	if (sign == 1)
		limit = LLONG_MAX;
	else
		limit = (unsigned long long)LLONG_MAX + 1ULL;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		if (result > (limit - (str[i] - '0')) / 10)
			return (0);
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (1);
}

long long	ft_atoll(const char *str)
{
	int			i;
	int			sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

int	ft_exit(t_cmd *cmd)
{
	char		**args;
	int			arg_count;
	int			i;
	long long	value;

	args = cmd->argv;
	if (!args)
		exit(0);
	arg_count = 0;
	i = 0;
	while (args[i])
	{
		arg_count++;
		i++;
	}
	if (arg_count == 1)
		exit(0);
	else if (arg_count > 2)
		exit(ft_handle_error(7, EXIT_FAILURE, NULL, NULL));
	else if (!ft_is_numeric(args[1]))
		exit(ft_handle_error(14, 255, NULL, NULL));
	else
	{
		value = ft_atoll(args[1]);
		exit((int)(value % 256));
	}
	return (0);
}
