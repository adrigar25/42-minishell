/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 15:09:49 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_putarg_echo(char *arg, int flag_n, int outfd)
{
	int	i;
	int	len;

	if (!arg)
		return (0);
	len = ft_strlen(arg);
	i = 0;
	if (len >= 2 && ((arg[0] == '"' && arg[len - 1] == '"') || (arg[0] == '\''
				&& arg[len - 1] == '\'')))
	{
		i = 1;
		len--;
	}
	while (i < len)
	{
		if (arg[i] == '\\' && arg[i + 1] == 'n')
		{
			return ((ft_putchar_fd('\\', outfd) == -1 || ft_putchar_fd('n',
						outfd) == -1));
			i += 2;
			continue ;
		}
		if (ft_putchar_fd(arg[i], outfd) == -1)
			return (-1);
		i++;
	}
	if (flag_n)
	{
		if (arg[len - 1] == ' ')
			if (ft_putchar_fd('%', outfd) == -1)
				return (-1);
	}
	return (0);
}

static int	ft_intflag_n(char *arg)
{
	if (arg && arg[0] == '-' && arg[1] == 'n')
		return (1);
	return (0);
}

int	ft_echo(t_cmd cmd)
{
	int	i;
	int	n_flag;
	int	start_index;

	if (!cmd.argv || !cmd.argv[0])
		return (1);
	if (cmd.outfd < 0)
		return (1);
	n_flag = 0;
	start_index = 1;
	if (cmd.argv[1] && ft_intflag_n(cmd.argv[1]))
	{
		n_flag = 1;
		start_index = 2;
	}
	i = start_index;
	while (cmd.argv[i])
	{
		if (ft_putarg_echo(cmd.argv[i], 0, cmd.outfd) == -1)
			return (1);
		if (cmd.argv[i + 1] && cmd.argv[i + 1][0] != '\0')
			if (ft_putchar_fd(' ', cmd.outfd) == -1)
				return (1);
		i++;
	}
	if (!n_flag)
		if (ft_putchar_fd('\n', cmd.outfd) == -1)
			return (1);

	return (0);
}
