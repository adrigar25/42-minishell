/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_builtins_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/10 16:43:33 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/18 16:43:13 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"
#include <sys/wait.h>
#include <unistd.h>

int	ft_exec_builtin(t_cmd *cmd, t_data **data)
{
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
