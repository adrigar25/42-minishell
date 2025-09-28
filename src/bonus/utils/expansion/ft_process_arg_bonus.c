/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 22:27:16 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int	j;
	int	start;

	j = 0;
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return ((*dst = ft_strdup(arg)) != NULL);
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if (j > start && !ft_copy_literal(dst, arg, start, j))
			return (0);
		if (arg[j] == '$')
		{
			if (arg[j + 1] == '?')
			{
				if (!ft_expand_exit_status(dst, &j, data))
					return (0);
			}
			else if (!ft_expand_env_var(dst, arg, &j, data))
				return (0);
		}
	}
	return (1);
}
