/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:16:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/10 16:50:50 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_change_env(char *key, char *value, char ***envp)
{
	int		i;
	char	*new_var;
	char	**old_envp;
	int		key_len;
	char	*temp;

	if (!key || !value || !envp)
		return ;
	setenv(key, value, 1);
	key_len = ft_strlen(key);
	temp = ft_strjoin(key, "=");
	if (!temp)
		return ;
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return ;
	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], key, key_len) == 0
			&& (*envp)[i][key_len] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = new_var;
			return ;
		}
		i++;
	}
	free(new_var);
}

int	ft_cd(char **argv, char ***envp)
{
	char	buf[PATH_MAX];
	char	*oldpwd;
	char	*target_dir;

	if (!argv[1])
	{
		target_dir = getenv("HOME");
		if (!target_dir)
		{
			ft_putstr_error("cd: HOME not set\n");
			return (1);
		}
	}
	else
		target_dir = argv[1];
	if (chdir(target_dir) != 0)
	{
		perror("cd");
		return (1);
	}
	oldpwd = getenv("PWD");
	if (getcwd(buf, sizeof(buf)))
	{
		if (oldpwd)
			ft_change_env("OLDPWD", oldpwd, envp);
		ft_change_env("PWD", buf, envp);
	}
	return (0);
}
