/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_setenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 17:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 18:20:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**ft_realloc_envp(char **envp, int new_size)
{
	char	**new_envp;
	int		i;

	new_envp = malloc(sizeof(char *) * (new_size + 1));
	if (!new_envp)
		return (NULL);
	i = 0;
	while (envp && envp[i] && i < new_size)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[i] = NULL;
	return (new_envp);
}

static int	ft_count_envp(char **envp)
{
	int	count;

	count = 0;
	while (envp && envp[count])
		count++;
	return (count);
}

int	ft_setenv(char *name, char *value, char ***envp)
{
	int		i;
	int		name_len;
	char	*new_var;
	char	*temp;
	char	**new_envp;
	int		envp_size;

	if (!name || !value || !envp || !*envp)
		return (-1);
	name_len = ft_strlen(name);
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], name, name_len) == 0
			&& (*envp)[i][name_len] == '=')
		{
			temp = ft_strjoin(name, "=");
			if (!temp)
				return (-1);
			new_var = ft_strjoin(temp, value);
			free(temp);
			if (!new_var)
				return (-1);
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return (0);
		}
		i++;
	}
	envp_size = ft_count_envp(*envp);
	new_envp = ft_realloc_envp(*envp, envp_size + 1);
	if (!new_envp)
		return (-1);
	temp = ft_strjoin(name, "=");
	if (!temp)
	{
		i = 0;
		while (new_envp[i])
			free(new_envp[i++]);
		free(new_envp);
		return (-1);
	}
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
	{
		i = 0;
		while (new_envp[i])
			free(new_envp[i++]);
		free(new_envp);
		return (-1);
	}
	new_envp[envp_size] = new_var;
	new_envp[envp_size + 1] = NULL;
	i = 0;
	while ((*envp)[i])
		free((*envp)[i++]);
	free(*envp);
	*envp = new_envp;
	return (0);
}
