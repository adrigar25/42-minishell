/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 17:38:05 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_unset(char **args, char ***envp)
{
	int		i;
	int		j;
	int		k;
	char	*tmp;

	if (!args || !envp || !*envp)
		return (1);
	i = 1;
	while (args[i])
	{
		tmp = args[i];
		j = 0;
		while ((*envp)[j])
		{
			if (!ft_strncmp((*envp)[j], tmp, ft_strlen(tmp)) && (*envp)[j][ft_strlen(tmp)] == '=')
			{
				free((*envp)[j]);
				k = j;
				while ((*envp)[k])
				{
					(*envp)[k] = (*envp)[k + 1];
					k++;
				}
				j--;
			}
			j++;
		}
		i++;
	}
	return (0);
}
