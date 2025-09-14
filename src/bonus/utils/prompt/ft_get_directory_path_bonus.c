/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_directory_path_bonus.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:44:08 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/14 15:18:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

char	*ft_get_directory_path(char **envp)
{
	char	*pwd;
	char	*home;
	char	*result;
	int		home_len;
	int		pwd_len;

	pwd = ft_getenv("PWD", envp);
	if (!pwd || !*pwd)
		return (ft_strdup("(unknown)"));
	home = ft_getenv("HOME", envp);
	if (!home)
		return (ft_strdup(pwd));
	home_len = ft_strlen(home);
	pwd_len = ft_strlen(pwd);
	if (pwd_len >= home_len && ft_strncmp(pwd, home, home_len) == 0)
	{
		if (pwd_len == home_len)
			return (ft_strdup("~"));
		else if (pwd[home_len] == '/')
		{
			result = malloc(pwd_len - home_len + 2);
			if (!result)
				return (ft_strdup(pwd));
			result[0] = '~';
			ft_strcpy(result + 1, pwd + home_len);
			return (result);
		}
	}
	return (ft_strdup(pwd));
}
