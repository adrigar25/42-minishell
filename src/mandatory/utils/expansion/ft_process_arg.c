/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/13 16:25:34 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	process_dollar(char **dst, char *arg, int *j, t_data *data)
{
	int	ret;
	int	arg_len;

	arg_len = ft_strlen(arg);
	if (arg[*j + 1] == '"' && ft_strchr(arg + *j + 2, '\''))
	{
		(*j)++;
		return (0);
	}
	if (arg[*j + 1] == '?')
	{
		if (!ft_expand_exit_status(dst, j, data))
			return (-1);
		return (1);
	}
	ret = ft_expand_env_var(dst, arg, j, data);
	if (ret == 0 && !(arg[0] == '"' && arg[arg_len - 1] == '"'))
		return (0);
	if (ret == -1)
		return (-1);
	return (1);
}

static int	process_segment(char **dst, char *arg, int *j, t_data *data)
{
	int	start;
	int	res;

	start = *j;
	while (arg[*j] && arg[*j] != '$')
		(*j)++;
	if (*j > start)
	{
		if (!ft_copy_literal(dst, arg, start, *j))
			return (-1);
	}
	if (arg[*j] == '$')
	{
		res = process_dollar(dst, arg, j, data);
		if (res == -1)
			return (-1);
		if (res == 0)
			return (0);
	}
	return (1);
}

int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int	j;
	int	ret;

	if (!arg)
		return (0);
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
	{
		*dst = ft_substr(arg, 0, ft_strlen(arg));
		return (*dst != NULL);
	}
	*dst = ft_strdup("");
	if (!*dst)
		return (0);
	j = 0;
	while (arg[j])
	{
		ret = process_segment(dst, arg, &j, data);
		if (ret == -1)
			return (0);
		if (ret == 0)
			continue ;
	}
	return (1);
}
