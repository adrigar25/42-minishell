/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_token.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:55:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:55:03 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_process_token(t_cmd **current_cmd, char **argv, int i, int *cmd_index)
{
	char	*clean_arg;

	if (ft_strcmp(argv[i], "|") == 0)
		return (ft_process_pipe(current_cmd, cmd_index, (*current_cmd)->data),
			i);
	else if ((ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], ">") == 0
			|| ft_strcmp(argv[i], ">>") == 0 || ft_strcmp(argv[i], "<<") == 0)
		&& (*current_cmd)->has_error == 0)
		return (ft_handle_redirection(*current_cmd, argv, i,
				(*current_cmd)->data));
	else
	{
		clean_arg = ft_remove_quotes(argv[i]);
		if (clean_arg)
			ft_add_arg_to_cmd(*current_cmd, clean_arg);
		return (i);
	}
}
