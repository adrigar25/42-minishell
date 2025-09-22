/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_env_expansion_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 12:00:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

static int	ft_append(char **dst, char *src)
{
	char	*old;

	old = *dst;
	*dst = ft_strjoin(old, src);
	free(old);
	return (*dst != NULL);
}

static int	ft_expand_exit_status(char **dst, int *j, t_data *data)
{
	char	*temp;

	temp = ft_itoa(data->last_exit_status);
	if (!temp)
		return (0);
	if (!ft_append(dst, temp))
	{
		free(temp);
		return (0);
	}
	free(temp);
	(*j)++;
	return (1);
}

static int	ft_expand_env(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	(*j)++;
	if (arg[*j] == '?')
		return (ft_expand_exit_status(dst, j, data));
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
	if (env_value && !ft_append(dst, env_value))
		return (0);
	return (1);
}

static int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int		j;
	int		start;
	char	*temp;

	j = 0;
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return ((*dst = ft_strdup(arg)) != NULL);
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if (j > start)
		{
			temp = ft_substr(arg, start, j - start);
			if (!ft_append(dst, temp))
			{
				free(temp);
				return (0);
			}
			free(temp);
		}
		if (arg[j] == '$' && !ft_expand_env(dst, arg, &j, data))
			return (0);
	}
	return (1);
}

char	**ft_handle_env_expansion(char **argv, t_data *data)
{
	int		i;
	char	**new_argv;

	if (!argv || !data || !data->envp)
		return (argv);
	new_argv = malloc(sizeof(char *) * (data->argc + 1));
	if (!new_argv)
		return (argv);
	i = 0;
	while (argv[i])
	{
		new_argv[i] = ft_strdup("");
		if (!new_argv[i] || !ft_process_arg(&new_argv[i], argv[i], data))
		{
			ft_free_matrix(new_argv);
			return (argv);
		}
		i++;
	}
	new_argv[i] = NULL;
	return (new_argv);
}
