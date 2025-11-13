/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_pwd_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 14:04:05 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/13 17:55:39 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*resolve_abs_target(const char *target_dir)
{
	if (target_dir && target_dir[0] == '/')
		return (ft_normalize_path((char *)target_dir));
	return (NULL);
}

static char	*resolve_relative_from_old(const char *oldpwd,
		const char *target_dir)
{
	char	*joined;
	char	*tmp;
	char	*normalized;

	if (!oldpwd || oldpwd[0] != '/')
		return (NULL);
	joined = ft_strjoin((char *)oldpwd, "/");
	if (!joined)
		return (NULL);
	if (target_dir)
		tmp = ft_strjoin(joined, (char *)target_dir);
	else
		tmp = ft_strdup(joined);
	free(joined);
	if (!tmp)
		return (NULL);
	normalized = ft_normalize_path(tmp);
	free(tmp);
	return (normalized);
}

static char	*fallback_newpwd(const char *target_dir)
{
	char	buf[4096];

	if (getcwd(buf, sizeof(buf)))
		return (ft_strdup(buf));
	if (target_dir)
		return (ft_strdup(target_dir));
	return (ft_strdup("/"));
}

void	ft_update_pwd_env(char *oldpwd, char *target_dir, char ***envp)
{
	char	*newpwd;

	newpwd = NULL;
	newpwd = resolve_abs_target(target_dir);
	if (!newpwd)
		newpwd = resolve_relative_from_old(oldpwd, target_dir);
	if (!newpwd)
		newpwd = fallback_newpwd(target_dir);
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, envp);
	ft_setenv("PWD", newpwd, envp);
	free(newpwd);
}
