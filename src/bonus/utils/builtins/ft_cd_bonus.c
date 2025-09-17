/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 17:16:33 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/16 21:07:47 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <errno.h>

int	ft_cd(char **argv, char ***envp)
{
	char	buf[4096];
	char	*oldpwd;
	char	*target_dir;

	if (!argv || !envp)
		return (1);
	if (!argv[1])
	{
		target_dir = ft_getenv("HOME", *envp);
		if (!target_dir)
			return (ft_handle_error(8, EXIT_FAILURE, NULL, NULL));
	}
	else if (ft_strcmp(argv[1], "~") == 0)
	{
		target_dir = getenv("HOME");
		if (!target_dir)
			return (ft_handle_error(8, EXIT_FAILURE, NULL, NULL));
	}
	else
		target_dir = argv[1];
	if (chdir(target_dir) != 0)
		return (ft_handle_error(11, EXIT_FAILURE, target_dir, strerror(errno)));
	oldpwd = getenv("PWD");
	if (getcwd(buf, sizeof(buf)))
	{
		if (oldpwd)
			ft_setenv("OLDPWD", oldpwd, envp);
		ft_setenv("PWD", buf, envp);
	}
	else
	{
		if (oldpwd)
			ft_setenv("OLDPWD", oldpwd, envp);
		ft_setenv("PWD", target_dir, envp);
	}
	return (0);
}
