/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_strings.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/07 00:56:51 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_split_strings(char ***cmds)
{
	int	i;
	int	j;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
	{
		j = 0;
		while (cmds[i][j])
		{
			free(cmds[i][j]);
			j++;
		}
		free(cmds[i]);
		i++;
	}
	free(cmds);
}

char	***ft_split_strings(const char **argv, const char *sep)
{
	int		count;
	char	***cmds;
	int		i;
	int		start;
	int		end;
	int		j;
	int		k;

	count = 0;
	i = 0;
	while (argv[i])
	{
		if (ft_strcmp(argv[i], sep) == 0)
			i++;
		else
		{
			count++;
			while (argv[i] && ft_strcmp(argv[i], sep) != 0)
				i++;
		}
	}
	cmds = malloc((count + 1) * sizeof(char **));
	if (!cmds)
		return (NULL);
	i = 0;
	j = 0;
	while (argv[i])
	{
		if (ft_strcmp(argv[i], sep) == 0)
			i++;
		else
		{
			start = i;
			while (argv[i] && ft_strcmp(argv[i], sep) != 0)
				i++;
			end = i;
			cmds[j] = malloc((end - start + 1) * sizeof(char *));
			if (!cmds[j])
			{
				free_split_strings(cmds);
				return (NULL);
			}
			k = 0;
			while (start < end)
			{
				cmds[j][k] = ft_strdup(argv[start]);
				if (!cmds[j][k])
				{
					free_split_strings(cmds);
					return (NULL);
				}
				k++;
				start++;
			}
			cmds[j][k] = NULL;
			j++;
		}
	}
	cmds[j] = NULL;
	return (cmds);
}
