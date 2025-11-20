/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/20 14:47:22 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_handle_quoted_after_dollar(char **dst, char *arg, int *j)
{
	int		start;
	int		end;
	char	*content;
	int		ret;

	if (!arg[*j + 1] || !arg[*j + 2])
	{
		(*j)++;
		return (ft_append(dst, "$"));
	}
	start = *j + 2;
	end = start;
	while (arg[end] && arg[end] != arg[*j + 1])
		end++;
	if (arg[end] != arg[*j + 1])
		return ((*j)++, ft_append(dst, "$"));
	content = ft_substr(arg, start, end - start);
	if (!content)
		return (ERROR);
	ret = ft_append(dst, content);
	free(content);
	*j = end + 1;
	return (ret);
}

static int	ft_expand_var(char **dst, char *arg, int *j, t_data *data)
{
	if (arg[*j + 1] == '"' && ft_strchr(arg + *j + 2, '\''))
	{
		(*j)++;
		return (SUCCESS);
	}
	if (arg[*j + 1] == '"')
		return (ft_handle_quoted_after_dollar(dst, arg, j));
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
