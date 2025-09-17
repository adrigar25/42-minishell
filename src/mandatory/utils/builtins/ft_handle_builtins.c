/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:43:33 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/17 12:08:11 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
#include <sys/wait.h>
#include <unistd.h>

int	ft_handle_builtins(t_cmd *cmd, t_data **data, t_cmd *cmd_list, pid_t *pids)
{
	pid_t	pid;
	int		status;

	status = 0;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (-1);
	if (cmd->infd != STDIN_FILENO || cmd->outfd != STDOUT_FILENO)
	{
		pid = fork();
		if (pid == 0)
		{
			ft_setup_child_io(cmd, cmd_list);
			if (ft_strcmp(cmd->argv[0], "echo") == 0)
				status = ft_echo(*cmd);
			else if (ft_strcmp(cmd->argv[0], "cd") == 0)
				status = ft_cd(cmd->argv, &(*data)->envp);
			else if (ft_strcmp(cmd->argv[0], "pwd") == 0)
				status = ft_pwd(*cmd);
			else if (ft_strcmp(cmd->argv[0], "export") == 0)
				status = ft_export(cmd->argv, &(*data)->envp);
			else if (ft_strcmp(cmd->argv[0], "unset") == 0)
				status = ft_unset(cmd->argv, &(*data)->envp);
			else if (ft_strcmp(cmd->argv[0], "env") == 0)
				status = ft_env(*cmd, (*data)->envp);
			else if (ft_strcmp(cmd->argv[0], "exit") == 0)
				ft_exit(cmd);
			exit(status);
		}
		else if (pid > 0)
		{
			if (pids)
				pids[cmd->index] = pid;
		}
		else
		{
			perror("fork");
			return (-1);
		}
		return (0);
	}
	// Builtin sin redirección, ejecuta en el proceso padre
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(*cmd));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd->argv, &(*data)->envp));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(*cmd));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd->argv, &(*data)->envp));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd->argv, &(*data)->envp));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(*cmd, (*data)->envp));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		ft_exit(cmd);
	return (-1);
}
