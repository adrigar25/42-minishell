/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_token_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:55:03 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 03:15:50 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

static int	is_redir_token(const char *token)
{
	if (!token)
		return (0);
	return (!ft_strcmp(token, "<") || !ft_strcmp(token, ">")
		|| !ft_strcmp(token, ">>") || !ft_strcmp(token, "<<"));
}

static int	is_pipe_like_token(const char *token)
{
	if (!token)
		return (0);
	return (ft_strcmp(token, "|") == 0 || ft_strcmp(token, "||") == 0
		|| ft_strcmp(token, "&&") == 0);
}

int	ft_process_token(t_cmd **current_cmd, char **argv, int i, int *cmd_index)
{
	char	*clean_arg;
	t_cmd	*new_cmd;
	int		k;
	int		count_nonop;

	if (!argv[i])
		return (i);
	k = 0;
	if (is_pipe_like_token(argv[i]))
	{
		ft_process_op(current_cmd, argv[i], cmd_index, (*current_cmd)->data);
		return (i + 1);
	}
	else if (is_redir_token(argv[i]) && (*current_cmd)->has_error == 0)
	{
		return (ft_handle_redirection(*current_cmd, argv, i,
				(*current_cmd)->data));
	}
	else
	{
		clean_arg = ft_remove_quotes(argv[i]);
		if (clean_arg)
			ft_add_arg_to_cmd(*current_cmd, clean_arg);
		return (i);
	}
	return (i);
}
