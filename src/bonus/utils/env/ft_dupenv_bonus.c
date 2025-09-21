/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dupenv_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:00:26 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 00:11:55 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

char	**ft_dupenv(char **envp)
{
	char	**envp_cpy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	envp_cpy = malloc((i + 1) * sizeof(char *));
	if (!envp_cpy)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		envp_cpy[i] = ft_strdup(envp[i]);
		if (!envp_cpy[i])
		{
			while (i > 0)
			{
				free(envp_cpy[--i]);
			}
			free(envp_cpy);
			return (NULL);
		}
		i++;
	}
	envp_cpy[i] = NULL;
	return (envp_cpy);
}
