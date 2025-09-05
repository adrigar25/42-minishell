/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/06 00:51:32 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exit(char **args)
{
	int	exit_code;

	if (!args[1])
		exit(0);
	else if (args[2])
	{
		ft_putstr_error("minishell: exit: too many arguments\n");
		exit(1);
	}
	else
	{
		if (!ft_is_number(args[1]))
		{
			ft_putstr_error("minishell: exit: numeric argument required\n");
			exit(255);
		}
		exit_code = ft_atoi(args[1]);
		exit(exit_code);
	}
}
