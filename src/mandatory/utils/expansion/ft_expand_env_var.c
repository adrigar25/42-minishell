/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_env_var.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:32:49 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_expand_env_var(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	(*j)++;
	start = *j;
	while (arg[*j] && (ft_isalnum(arg[*j]) || arg[*j] == '_'))
		(*j)++;
	if (*j == start)
		return (ft_append(dst, "$"));
	env_name = ft_substr(arg, start, *j - start);
	if (!env_name)
		return (0);
	env_value = ft_getenv(env_name, data->envp);
	free(env_name);
	if (env_value)
		if (!ft_append(dst, (char *)env_value))
			return (0);
	return (1);
}
