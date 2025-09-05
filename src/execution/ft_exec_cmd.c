/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/06 00:50:48 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(char **args, int fd_in, int fd_out, char **envp)
{
	char	*path;

	if (!args || !args[0])
		return (1);
	ft_handle_redir(args, &fd_in, &fd_out);
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = get_cmd_path(args[0]);
	if (!args || !args[0] || !path || access(path, X_OK) == -1)
	{
		if (path)
			printf("minishell: %s\n", args[0]);
		else
			printf("minishell: command not found: %s\n", args[0]);
		if (path)
			exit(126);
		else
			exit(127);
	}
	if (fd_in != 0)
		ft_redir_io(fd_in, 0);
	ft_redir_io(fd_out, 1);
	execve(path, args, envp);
	return (0);
}
