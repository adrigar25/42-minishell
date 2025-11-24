/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_normalize_path.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:44:59 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/24 16:57:04 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_build_stack(char **parts, char **stack)
{
	int	i[2];

	i[0] = 0;
	i[1] = 0;
	while (parts[i[0]])
	{
		if (ft_strcmp(parts[i[0]], ".") == 0)
		{
			i[0]++;
			continue ;
		}
		else if (ft_strcmp(parts[i[0]], "..") == 0)
		{
			if (i[1] > 0)
			{
				free(stack[i[1] - 1]);
				i[1]--;
			}
		}
		else
			stack[i[1]++] = ft_strdup(parts[i[0]]);
		i[0]++;
	}
	return (i[1]);
}

static char	*build_path_from_stack(char **stack, int top)
{
	char	*res;
	char	*tmp;
	int		i;

	if (top == 0)
		return (ft_strdup("/"));
	res = ft_strdup("");
	i = 0;
	while (i < top)
	{
		tmp = ft_strjoin(res, "/");
		free(res);
		res = ft_strjoin(tmp, stack[i]);
		free(tmp);
		i++;
	}
	return (res);
}

char	*ft_normalize_path(char *path)
{
	char	**parts;
	char	**stack;
	char	*res;
	int		top;

	if (!path)
		return (NULL);
	parts = ft_split(path, '/');
	if (!parts)
		return (ft_strdup("/"));
	stack = malloc(sizeof(char *) * (ft_matrix_size(parts) + 1));
	if (!stack)
		return (ft_free_matrix(parts), ft_strdup("/"));
	top = ft_build_stack(parts, stack);
	res = build_path_from_stack(stack, top);
	while (top > 0)
	{
		free(stack[top - 1]);
		top--;
	}
	free(stack);
	ft_free_matrix(parts);
	return (res);
}
