/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:16:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/14 15:18:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

int	ft_cd(char **argv, char ***envp)
{
	char	buf[PATH_MAX];
	char	*oldpwd;
	char	*target_dir;

	if (!argv || !envp)
		return (1);
	if (!argv[1])
	{
		target_dir = getenv("HOME");
		if (!target_dir)
		{
			ft_fprintf(2, ERROR_HOME_NOT_SET);
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
			ft_setenv("OLDPWD", oldpwd, envp);
		ft_setenv("PWD", buf, envp);
	}
	else
	{
		perror("getcwd");
		if (oldpwd)
			ft_setenv("OLDPWD", oldpwd, envp);
		ft_setenv("PWD", target_dir, envp);
	}
	return (0);
}
