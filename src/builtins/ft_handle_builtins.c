/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/06 10:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 18:45:41 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_execute_builtin(t_cmd *cmd, char ***envp)
{
	if (ft_strcmp(cmd->argv[0], "echo") == 0)
		return (ft_echo(*cmd));
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (ft_cd(cmd->argv, *envp));
	if (ft_strcmp(cmd->argv[0], "pwd") == 0)
		return (ft_pwd(cmd->argv));
	if (ft_strcmp(cmd->argv[0], "export") == 0)
		return (ft_export(cmd->argv, envp));
	if (ft_strcmp(cmd->argv[0], "unset") == 0)
		return (ft_unset(cmd->argv, envp));
	if (ft_strcmp(cmd->argv[0], "env") == 0)
		return (ft_env(*envp));
	if (ft_strcmp(cmd->argv[0], "exit") == 0)
		return (ft_exit(cmd->argv));
	return (1);
}

int	ft_handle_builtins(t_cmd *cmd, char ***envp)
{
	if (ft_execute_builtin(cmd, envp) == 1)
		return (1);
	return (0);
}
