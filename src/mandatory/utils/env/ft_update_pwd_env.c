/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_update_pwd_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 14:04:05 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 14:09:01 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Updates the PWD and OLDPWD environment variables.
 *          If getcwd fails, it sets PWD to the provided target_dir.
 *
 * SPANISH: Actualiza las variables de entorno PWD y OLDPWD.
 *          Si getcwd falla, establece PWD al directorio objetivo proporcionado.
 *
 * @param oldpwd      The previous working directory to set as OLDPWD.
 *                    Can be NULL. /
 *                    El directorio de trabajo anterior para establecer
 *							como OLDPWD. Puede ser NULL.
 *
 * @param target_dir  The target directory to set as PWD if getcwd fails. /
 *                    El directorio objetivo para establecer como PWD si
 * 					getcwd falla.
 *
 * @param envp        Pointer to the environment variable array. /
 *                    Puntero al array de variables de entorno.
 */
/*
 * Normalize a path string (must be absolute or will be treated as absolute
 * when joined). This routine removes '.' components and resolves '..'
 * syntactically without calling realpath, so it does not resolve symlinks.
 */
static char	*ft_normalize_path(const char *path)
{
	char	**parts;
	char	**stack;
	int		i;
	int		top;
	char	*res;
	char	*tmp;

	if (!path)
		return (NULL);
	parts = ft_split(path, '/');
	if (!parts)
		return (ft_strdup("/"));
	i = 0;
	while (parts[i])
		i++;
	stack = malloc(sizeof(char *) * (i + 1));
	if (!stack)
	{
		ft_free_matrix(parts);
		return (ft_strdup("/"));
	}
	top = 0;
	i = 0;
	while (parts[i])
	{
		if (ft_strcmp(parts[i], ".") == 0)
		{
			i++;
			continue ;
		}
		else if (ft_strcmp(parts[i], "..") == 0)
		{
			if (top > 0)
			{
				free(stack[top - 1]);
				top--;
			}
		}
		else
		{
			stack[top++] = ft_strdup(parts[i]);
		}
		i++;
	}
	if (top == 0)
	{
		res = ft_strdup("/");
	}
	else
	{
		res = ft_strdup("");
		for (i = 0; i < top; i++)
		{
			tmp = ft_strjoin(res, "/");
			free(res);
			res = ft_strjoin(tmp, stack[i]);
			free(tmp);
		}
	}
	i = 0;
	while (i < top)
	{
		free(stack[i]);
		i++;
	}
	free(stack);
	ft_free_matrix(parts);
	return (res);
}

void	ft_update_pwd_env(char *oldpwd, char *target_dir, char ***envp)
{
	char	*newpwd;
	char	*joined;
	char	*tmp;
	char	buf[4096];

	newpwd = NULL;
	if (target_dir && target_dir[0] == '/')
	{
		newpwd = ft_normalize_path(target_dir);
	}
	else if (oldpwd && oldpwd[0] == '/')
	{
		joined = ft_strjoin(oldpwd, "/");
		if (joined)
		{
			tmp = ft_strjoin(joined, target_dir ? target_dir : "");
			free(joined);
			if (tmp)
			{
				newpwd = ft_normalize_path(tmp);
				free(tmp);
			}
		}
	}
	if (!newpwd)
	{
		if (getcwd(buf, sizeof(buf)))
			newpwd = ft_strdup(buf);
		else
			newpwd = target_dir ? ft_strdup(target_dir) : ft_strdup("/");
	}
	if (oldpwd)
		ft_setenv("OLDPWD", oldpwd, envp);
	ft_setenv("PWD", newpwd, envp);
	free(newpwd);
}
