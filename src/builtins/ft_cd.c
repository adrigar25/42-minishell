/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:16:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/09 20:50:41 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_pwd_env(char **envp)
{
	char	cwd[1024];
	char	*old_pwd;

	old_pwd = getenv("PWD");
	if (old_pwd)
		setenv("OLDPWD", old_pwd, 1);
	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return ;
	setenv("PWD", cwd, 1);
}

int	ft_cd(char **args, char **envp)
{
	char	*home;
	char	*error_msg;
	int		i;

	if (!args[1])
	{
		home = NULL;
		i = 0;
		while (envp[i])
		{
			if (ft_strncmp(envp[i], "HOME=", 5) == 0)
			{
				home = envp[i] + 5;
				break ;
			}
			i++;
		}
		if (home)
		{
			if (chdir(home) == -1)
			{
				perror("minishell: cd");
				return (1);
			}
			update_pwd_env(envp);
		}
		else
		{
			ft_putstr_error(ERROR_HOME_NOT_SET);
			return (1);
		}
	}
	else if (args[2])
	{
		ft_putstr_error(ERROR_TOO_MANY_ARGS);
		return (1);
	}
	else
	{
		if (chdir(args[1]) == -1)
		{
			perror("minishell: cd");
			return (1);
		}
		update_pwd_env(envp);
	}
	return (0);
}
