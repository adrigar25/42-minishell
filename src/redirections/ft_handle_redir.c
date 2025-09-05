/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/06 00:05:41 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

int	ft_handle_redir(char **args, int *fd_in, int *fd_out)
{
	int		i;
	int		j;
	int		fd;
	char	**clean_args;
	int		clean_index;

	if (!args || !fd_in || !fd_out)
		return (-1);
	// Count non-redirection arguments
	i = 0;
	clean_index = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0 || ft_strcmp(args[i], ">") == 0
			|| ft_strcmp(args[i], ">>") == 0)
		{
			i += 2; // Skip redirection operator and filename
		}
		else
		{
			clean_index++;
			i++;
		}
	}
	// Create clean args array
	clean_args = malloc((clean_index + 1) * sizeof(char *));
	if (!clean_args)
		return (-1);
	// Process redirections and build clean args
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "<") == 0)
		{
			if (!args[i + 1])
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				free(clean_args);
				return (-1);
			}
			fd = ft_handle_infile(args[i + 1]);
			if (fd == -1)
			{
				free(clean_args);
				return (-1);
			}
			*fd_in = fd;
			i += 2; // Skip < and filename
		}
		else if (ft_strcmp(args[i], ">>") == 0)
		{
			if (!args[i + 1])
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				free(clean_args);
				return (-1);
			}
			fd = ft_handle_outfile(args[i + 1], 1);
			if (fd == -1)
			{
				free(clean_args);
				return (-1);
			}
			*fd_out = fd;
			i += 2; // Skip >> and filename
		}
		else if (ft_strcmp(args[i], ">") == 0)
		{
			if (!args[i + 1])
			{
				printf("minishell: syntax error near unexpected token `newline'\n");
				free(clean_args);
				return (-1);
			}
			fd = ft_handle_outfile(args[i + 1], 0);
			if (fd == -1)
			{
				free(clean_args);
				return (-1);
			}
			*fd_out = fd;
			i += 2; // Skip > and filename
		}
		else
		{
			clean_args[j] = args[i];
			j++;
			i++;
		}
	}
	clean_args[j] = NULL;
	// Copy clean args back to original array
	i = 0;
	while (clean_args[i])
	{
		args[i] = clean_args[i];
		i++;
	}
	args[i] = NULL;
	free(clean_args);
	return (0);
}
