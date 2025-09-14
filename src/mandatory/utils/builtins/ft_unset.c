/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 14:36:02 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
			printf("%c", args[i][ft_strlen(args[i]) - 1]);
			if (args[i][ft_strlen(args[i]) - 1] == '='
				|| args[i][ft_strlen(args[i]) - 1] == '-')
			{
				ft_fprintf(2,
					"minishell: unset: `%s': not a valid identifier\n",
					args[i]);
				return (1);
				break ;
			}
			if (!ft_strncmp((*envp)[j], tmp, ft_strlen(tmp))
				&& (*envp)[j][ft_strlen(tmp)] == '=')
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
