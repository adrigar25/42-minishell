/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/24 17:55:11 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_expand_var(char **dst, char *arg, int *j, t_data *data)
{
	int	start;
	int	end;

	if (arg[*j + 1] == '"')
	{
		start = *j + 2;
		end = start;
		while (arg[end] && arg[end] != '"')
			end++;
		if (arg[end] == '"')
		{
			if (ft_copy_literal(dst, arg, start, end) == ERROR)
				return (ERROR);
			*j = end + 1;
			return (SUCCESS);
		}
	}
	if (arg[*j + 1] == '?')
		return (ft_expand_exit_status(dst, j, data));
	if (arg[*j + 1] == '$')
		return (ft_expand_pid(dst, j, data));
	return (ft_expand_env_var(dst, arg, j, data));
}

char	*ft_process_arg(char *arg, t_data *data)
{
	int		j;
	char	*dst;
	int		start;

	if (!arg)
		return (NULL);
	if (arg[0] == '$' && arg[1] == '"' && arg[ft_strlen(arg) - 1] == '"')
		return (ft_substr(arg, 1, ft_strlen(arg) - 1));
	if (arg[0] == '$' && arg[1] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return (ft_substr(arg, 2, ft_strlen(arg) - 3));
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return (ft_substr(arg, 0, ft_strlen(arg)));
	dst = ft_strdup("");
	if (!dst)
		return (NULL);
	j = 0;
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if ((ft_copy_literal(&dst, arg, start, j) == ERROR) || (arg[j] == '$'
				&& (ft_expand_var(&dst, arg, &j, data) == ERROR)))
		{
			free(dst);
			return (NULL);
		}
	}
	return (dst);
}
