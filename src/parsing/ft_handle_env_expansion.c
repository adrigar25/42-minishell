/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_env_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 10:26:58 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_handle_env_expansion(int argc, char **argv, char **envp)
{
	int		i;
	int		j;
	char	**new_argv;
	int		start;
	int		len;
	char	*temp;
	char	*env_name;
	char	*env_value;

	if (!argv || !envp)
		return (argv);
	new_argv = malloc(sizeof(char *) * (argc + 1));
	if (!new_argv)
		return (argv);
	i = 0;
	while (argv[i])
	{
		j = 0;
		while (argv[i][j])
		{
			start = j;
			while (argv[i][j] && argv[i][j] != '$')
				j++;
			temp = ft_substr(argv[i], start, j - start);
			new_argv[i] = ft_strjoin(new_argv[i], temp);
			free(temp);
			if (!new_argv[i])
			{
				free(new_argv);
				return (NULL);
			}
			if (argv[i][j] == '$')
			{
				j++;
				start = j;
				while (argv[i][j] && argv[i][j] != '$' && argv[i][j] != ' '
					&& argv[i][j] != '"' && argv[i][j] != '/')
					j++;
				env_name = ft_substr(argv[i], start, j - start);
				env_value = ft_getenv(env_name, envp);
				if (env_value)
					new_argv[i] = ft_strjoin(new_argv[i], env_value);
				free(env_name);
				free(env_value);
			}
		}
		printf("\n");
		i++;
	}
	new_argv[i] = NULL;
	return (new_argv);
}
