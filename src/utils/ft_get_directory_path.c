/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_get_directory_path.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 17:44:08 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/06 00:58:01 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_directory_path(char *dest)
{
	char	cwd[1024];
	char	*home;
	char	*result;
	int		home_len;
	int		cwd_len;

	if (getcwd(cwd, sizeof(cwd)) == NULL)
		return (ft_strdup("(unknown)"));
	home = getenv("HOME");
	if (!home)
		return (ft_strdup(cwd));
	home_len = ft_strlen(home);
	cwd_len = ft_strlen(cwd);
	// Si el directorio actual está dentro de HOME, mostrar con ~
	if (cwd_len >= home_len && ft_strncmp(cwd, home, home_len) == 0)
	{
		if (cwd_len == home_len) // Estamos en HOME exactamente
			return (ft_strdup("~"));
		else if (cwd[home_len] == '/') // Estamos en un subdirectorio de HOME
		{
			result = malloc(cwd_len - home_len + 2);
			if (!result)
				return (ft_strdup(cwd));
			result[0] = '~';
			ft_strcpy(result + 1, cwd + home_len);
			return (result);
		}
	}
	// Si no está en HOME, mostrar path completo
	return (ft_strdup(cwd));
}
