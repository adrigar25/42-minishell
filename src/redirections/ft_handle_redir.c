/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_redir.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 20:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/07 01:15:50 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>

static int	ft_is_redir_token(const char *token)
{
	return (ft_strcmp(token, "<") == 0 || ft_strcmp(token, ">") == 0
		|| ft_strcmp(token, ">>") == 0);
}

static int	handle_redir_error(int error_code, char **clean_args)
{
	if (error_code == 1)
		printf("minishell: syntax error near unexpected token `newline'\n");
	else if (error_code == 2)
		printf("minishell: no such file or directory\n");
	else if (error_code == 3)
		printf("minishell: permission denied\n");
	else if (error_code == 4)
		printf("minishell: failed to create file\n");
	if (clean_args)
		free(clean_args);
	return (-1);
}

static int	ft_process_redir(char **args, int *i, int *fd_in, int *fd_out,
		char **clean_args)
{
	int	res;
	int	fd;

	if (!args[*i + 1])
		return (handle_redir_error(1, clean_args));
	if (ft_strcmp(args[*i], "<") == 0)
	{
		fd = ft_handle_infile(args[*i + 1]);
		if (fd == -1)
			return (handle_redir_error(2, clean_args));
		if (*fd_in != 0 && *fd_in != -1)
			close(*fd_in);
		*fd_in = fd;
	}
	else if (ft_strcmp(args[*i], ">") == 0 || ft_strcmp(args[*i], ">>") == 0)
	{
		fd = ft_handle_outfile(args[*i + 1], ft_strcmp(args[*i], ">>") == 0);
		if (fd == -1)
			return (handle_redir_error(4, clean_args));
		if (*fd_out != 1 && *fd_out != -1)
			close(*fd_out);
		*fd_out = fd;
	}
	else
		return (handle_redir_error(1, clean_args));
	*i += 2;
	return (0);
}

int	ft_handle_redir(char **args, int *fd_in, int *fd_out)
{
	int		i;
	int		j;
	int		fd;
	char	**clean_args;
	int		clean_index;
	int		res;

	if (!args || !fd_in || !fd_out)
		return (-1);
	i = 0;
	clean_index = 0;
	while (args[i])
	{
		if (ft_is_redir_token(args[i]))
			i++;
		else
			clean_index++;
		i++;
	}
	clean_args = malloc((clean_index + 1) * sizeof(char *));
	if (!clean_args)
		return (-1);
	i = 0;
	j = 0;
	while (args[i])
	{
		if (ft_is_redir_token(args[i]))
		{
			res = ft_process_redir(args, &i, fd_in, fd_out, clean_args);
			if (res != 0)
				return (res);
		}
		else
		{
			clean_args[j] = args[i];
			j++;
			i++;
		}
	}
	clean_args[j] = NULL;
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
