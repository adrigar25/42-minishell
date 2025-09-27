/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_redir.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:26 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:51:26 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

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
