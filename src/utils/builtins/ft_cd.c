/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:16:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/12 19:35:56 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
			ft_putstr_error("cd: HOME not set\n");
			return (1);
		}
	}
	else
		target_dir = argv[1];
	if (argv[1] && argv[2])
	{
		ft_putstr_error("cd: too many arguments\n");
		return (1);
	}
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
