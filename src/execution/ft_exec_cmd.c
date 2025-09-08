/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/08 14:50:57 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(char **args, int fd_in, int fd_out, char **envp)
{
	char	*path;

	if (!args || !args[0])
		return (1);
	if (ft_strchr(args[0], '/'))
		path = ft_strdup(args[0]);
	else
		path = get_cmd_path(args[0]);
	if (!args || !args[0] || !path || access(path, X_OK) == -1)
	{
		if (path)
			printf(ERROR_PERMISSION_DENIED, args[0]);
		else
			printf(ERROR_COMMAND_NOT_FOUND, args[0]);
		if (path)
			exit(EXIT_PERMISSION_DENIED);
		else
			exit(EXIT_COMMAND_NOT_FOUND);
	}
	if (fd_in != 0)
		ft_redir_io(fd_in, 0);
	ft_redir_io(fd_out, 1);
	execve(path, args, envp);
	return (0);
}
