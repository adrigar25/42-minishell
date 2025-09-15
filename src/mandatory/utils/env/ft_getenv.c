/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 10:26:32 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/15 20:55:52 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_getenv(const char *name, char **envp)
{
	int		i;
	char	*var;

	if (!name || !envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		var = ft_strchr(envp[i], '=');
		if (var)
		{
			if (ft_strncmp(envp[i], name, var - envp[i]) == 0)
				return (var + 1);
		}
		else
		{
			if (ft_strcmp(envp[i], name) == 0)
				return ("");
		}
		i++;
	}
	return (NULL);
}
