/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_env_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/20 00:39:21 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_expand_env_var(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	if (!arg[*j + 1] || arg[*j + 1] == '\'' || arg[*j + 1] == '"'
		|| arg[*j + 1] == ' ' || arg[*j + 1] == '\t')
	{
		(*j)++;
		return (ft_append(dst, "$"));
	}
	(*j)++;
	start = *j;
	while (arg[*j] && (ft_isalnum(arg[*j]) || arg[*j] == '_'))
		(*j)++;
	if (*j == start)
		return (ft_append(dst, "$"));
	env_name = ft_substr(arg, start, *j - start);
	env_value = ft_getenv(env_name, data->envp);
	free(env_name);
	if (!env_value || env_value[0] == '\0')
		return (SUCCESS);
	return (ft_append(dst, env_value));
}
