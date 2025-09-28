/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:26 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 17:53:52 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

/**
 * ENGLISH: Checks if a token is a redirection operator.
 *
 * SPANISH: Verifica si un token es un operador de redirección.
 *
 * @param token The token to check.
 *              El token a verificar.
 *
 * @returns 1 if the token is a redirection operator, 0 otherwise.
 *          1 si el token es un operador de redirección, 0 en caso contrario.
 */
int	ft_handle_redirection(t_cmd *cmd, char **argv, int i, t_data *data)
{
	char	*clean_arg;
	int		fd;

	clean_arg = ft_remove_quotes(argv[i + 1]);
	if (!clean_arg)
		clean_arg = argv[i + 1];
	fd = ft_get_fd_from_token(argv[i], clean_arg);
	if (fd != -1)
		ft_assign_fd(cmd, argv[i], fd);
	else
	{
		data->last_exit_status = 1;
		cmd->has_error = 1;
	}
	if (clean_arg != argv[i + 1])
		free(clean_arg);
	return (i + 1);
}
