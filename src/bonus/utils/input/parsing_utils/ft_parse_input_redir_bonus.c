/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_input_redir_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:51:26 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/28 15:05:13 by agarcia          ###   ########.fr       */
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
	return (!ft_strcmp(token, "|") || !ft_strcmp(token, "||")
		|| !ft_strcmp(token, "&&"));
}

static int	is_ambiguous_redir(char **argv, int i, t_data *data)
{
	int	matches;

	if (!argv[i + 1])
		return (0);
	if (ft_strcmp(argv[i], "<<") == 0)
		return (0);
	matches = ft_count_matches(argv[i + 1]);
	if (ft_has_wildcards(argv[i + 1]) && matches != 1)
		return (1);
	return (0);
}

int	ft_handle_redirection(t_cmd *cmd, char **argv, int i, t_data *data)
{
	char	*clean_arg;
	int		fd;
	int		k;
	int		count_nonop;

	if (is_ambiguous_redir(argv, i, data) == 1)
	{
		data->last_exit_status = ft_handle_error(12, 1, argv[i + 1], NULL);
		cmd->has_error = 1;
		return (i + 2);
	}
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
	return (i + 2);
}
