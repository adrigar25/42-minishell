/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/09 19:32:27 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(t_cmd *cmd, char **envp)
{
	char	*path;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	while (cmd)
	{
		if (ft_handle_builtins(cmd, &envp) == 0)
			return (0);
		path = get_cmd_path(cmd->argv[0]);
		if (!path)
		{
			printf(ERROR_COMMAND_NOT_FOUND, cmd->argv[0]);
			return (127);
		}
		if (execve(path, cmd->argv, envp) == -1)
		{
			perror("execve");
			free(path);
			return (1);
		}
		free(path);
		cmd = cmd->next;
	}
	return (0);
}
