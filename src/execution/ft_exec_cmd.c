/* ***************************************************	execve(path, cmd->argv,
		data->envp);********************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exec_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 16:11:51 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/10 11:47:47 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_exec_cmd(t_cmd *cmd, t_data *data)
{
	char	*path;
	char	*error_msg;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (1);
	// No hacer redirecciones aquí si ya se hicieron en el proceso padre
	// Las redirecciones para pipes se manejan en ft_minishell.c
	if (ft_strchr(cmd->argv[0], '/'))
		path = ft_strdup(cmd->argv[0]);
	else
		path = get_cmd_path(cmd->argv[0]);
	if (!path || access(path, X_OK) == -1)
	{
		if (path)
		{
			error_msg = ERROR_PERMISSION_DENIED;
			ft_putstr_error(error_msg);
			free(error_msg);
		}
		else
		{
			error_msg = ft_strjoin(ERROR_COMMAND_NOT_FOUND, cmd->argv[0]);
			ft_putstr_error(error_msg);
			free(error_msg);
		}
		free(path);
		exit(127);
	}
	execve(path, cmd->argv, data->envp);
	perror("minishell: execve");
	free(path);
	exit(127);
}
