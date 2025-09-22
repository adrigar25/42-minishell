/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 14:18:58 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 16:35:43 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Checks if the given command is a built-in shell command.
 *
 * SPANISH: Verifica si el comando dado es un comando interno del shell.
 *
 * @param current   The current command to check. /
 *                  El comando actual a verificar.
 *
 * @returns 1 if the command is a built-in, 0 otherwise. /
 *          1 si el comando es un built-in, 0 en caso contrario.
 */
int	ft_is_builtin(t_cmd *current)
{
	char	*cmd;

	if (!current || !current->argv || !current->argv[0])
		return (0);
	cmd = current->argv[0];
	if (ft_strcmp(cmd, "echo") == 0 || ft_strcmp(cmd, "cd") == 0
		|| ft_strcmp(cmd, "pwd") == 0 || ft_strcmp(cmd, "export") == 0
		|| ft_strcmp(cmd, "unset") == 0 || ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}
