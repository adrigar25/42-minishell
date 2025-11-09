/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/08 02:13:10 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * Procesa una cadena de argumento, expandiendo variables de entorno y `$?`.
 * Si una expansión falla o no existe:
 *   - Si el argumento estaba entre comillas → se expande a vacío ("").
 *   - Si no estaba entre comillas → se omite (sin espacio extra).
 */
int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int	j;
	int	in_double_quotes;
	int	in_single_quotes;
	int	ret;
	int	start;

	j = 0;
	in_double_quotes = (arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"');
	in_single_quotes = (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'');
	if (in_single_quotes)
	{
		*dst = ft_substr(arg, 0, ft_strlen(arg));
		return (*dst != NULL);
	}
	*dst = ft_strdup("");
	if (!*dst)
		return (0);
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if (j > start && !ft_copy_literal(dst, arg, start, j))
			return (0);
		if (arg[j] == '$')
		{
			if (arg[j + 1] == '"' && ft_strchr(arg + j + 2, '\''))
			{
				j++;
				continue ;
			}
			if (arg[j + 1] == '?')
			{
				if (!ft_expand_exit_status(dst, &j, data))
					return (0);
			}
			else
			{
				ret = ft_expand_env_var(dst, arg, &j, data);
				if (ret == 0 && !in_double_quotes)
					continue ;
				if (ret == -1)
					return (0);
			}
		}
	}
	return (1);
}
