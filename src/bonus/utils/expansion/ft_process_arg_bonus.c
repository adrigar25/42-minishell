/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/17 23:10:56 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_expand_var(char **dst, char *arg, int *j, t_data *data)
{
	if (arg[*j + 1] == '"' && ft_strchr(arg + *j + 2, '\''))
	{
		(*j)++;
		return (SUCCESS);
	}
	if (arg[*j + 1] == '?')
		return (ft_expand_exit_status(dst, j, data));
	return (ft_expand_env_var(dst, arg, j, data));
}

char	*ft_process_arg(char *arg, t_data *data)
{
	int		j;
	char	*dst;
	int		start;

	if (!arg)
		return (NULL);
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
