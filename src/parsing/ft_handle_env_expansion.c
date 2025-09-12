/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_env_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 17:37:58 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <ctype.h>

char	**ft_handle_env_expansion(char **argv, t_data *data)
{
	int		i;
	int		j;
	char	**new_argv;
	int		start;
	int		len;
	char	*temp;
	char	*env_name;
	char	*env_value;
	char	*old_str;

	if (!argv || !data || !data->envp)
		return (argv);
	new_argv = malloc(sizeof(char *) * (data->argc + 1));
	if (!new_argv)
		return (argv);
	i = 0;
	while (argv[i])
	{
		new_argv[i] = ft_strdup("");
		if (!new_argv[i])
		{
			free(new_argv);
			return (argv);
		}
		j = 0;
		if (argv[i][j] == '\'' && argv[i][ft_strlen(argv[i]) - 1] == '\'')
		{
			free(new_argv[i]);
			new_argv[i] = ft_strdup(argv[i]);
			if (!new_argv[i])
			{
				free(new_argv);
				return (argv);
			}
			i++;
			continue ;
		}
		while (argv[i][j])
		{
			start = j;
			while (argv[i][j] && argv[i][j] != '$')
				j++;
			if (j > start)
			{
				temp = ft_substr(argv[i], start, j - start);
				old_str = new_argv[i];
				new_argv[i] = ft_strjoin(old_str, temp);
				free(old_str);
				free(temp);
				if (!new_argv[i])
				{
					free(new_argv);
					return (argv);
				}
			}
			if (argv[i][j] == '$')
			{
				j++;
				if (argv[i][j] == '?')
				{
					temp = ft_itoa(data->last_exit_status);
					if (!temp)
					{
						free(new_argv);
						return (argv);
					}
					old_str = new_argv[i];
					new_argv[i] = ft_strjoin(old_str, temp);
					free(old_str);
					free(temp);
					if (!new_argv[i])
					{
						free(new_argv);
						return (argv);
					}
					j++;
					continue ;
				}
				start = j;
				while (argv[i][j] && (isalnum(argv[i][j]) || argv[i][j] == '_'))
					j++;
				if (j > start)
				{
					env_name = ft_substr(argv[i], start, j - start);
					env_value = ft_getenv(env_name, data->envp);
					if (env_value)
					{
						old_str = new_argv[i];
						new_argv[i] = ft_strjoin(old_str, env_value);
						free(old_str);
						if (!new_argv[i])
						{
							free(env_name);
							free(new_argv);
							return (NULL);
						}
					}
					free(env_name);
				}
				else
				{
					old_str = new_argv[i];
					new_argv[i] = ft_strjoin(old_str, "$");
					free(old_str);
					if (!new_argv[i])
					{
						free(new_argv);
						return (argv);
					}
				}
			}
		}
		i++;
	}
	new_argv[i] = NULL;
	return (new_argv);
}
