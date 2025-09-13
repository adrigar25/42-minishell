/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cpyenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 18:00:26 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/12 20:06:52 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_cpyenv(char ***envp_cpy, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	*envp_cpy = malloc((i + 1) * sizeof(char *));
	if (!*envp_cpy)
		return (1);
	i = 0;
	while (envp[i])
	{
		(*envp_cpy)[i] = ft_strdup(envp[i]);
		if (!(*envp_cpy)[i])
		{
			while (i > 0)
			{
				free((*envp_cpy)[--i]);
			}
			free(*envp_cpy);
			return (1);
		}
		i++;
	}
	(*envp_cpy)[i] = NULL;
	return (0);
}
