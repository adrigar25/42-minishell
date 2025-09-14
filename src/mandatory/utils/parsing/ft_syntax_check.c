/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:25:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:45:32 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_redir(const char *s)
{
	return (ft_strcmp(s, "<") == 0 || ft_strcmp(s, ">") == 0 || ft_strcmp(s,
			">>") == 0 || ft_strcmp(s, "<<") == 0);
}

static int	is_pipe_token(const char *s)
{
	return (ft_strcmp(s, "|") == 0);
}

static int	is_logical_op(const char *s)
{
	return (ft_strcmp(s, "||") == 0 || ft_strcmp(s, "&&") == 0);
}

static int	is_ampersand(const char *s)
{
	return (ft_strcmp(s, "&") == 0);
}

static int	is_pipe_like(const char *s)
{
	return (is_pipe_token(s) || is_logical_op(s) || is_ampersand(s));
}

int	ft_check_syntax_errors(char **argv, int argc)
{
	int	i;

	if (!argv || argc == 0)
		return (0);
	if (is_pipe_token(argv[0]))
		return (ft_handle_error(6, 2, argv[0], NULL));
	if (is_logical_op(argv[0]) || is_ampersand(argv[0]))
		return (ft_handle_error(6, 2, argv[0], NULL));
	if (is_redir(argv[0]) && argc == 1)
		return (ft_handle_error(5, 2, NULL, NULL));
	i = 0;
	while (i < argc)
	{
		if (is_pipe_like(argv[i]))
		{
			if (i == argc - 1)
				return (ft_handle_error(5, 2, NULL, NULL));
			if (is_pipe_like(argv[i + 1]))
				return (ft_handle_error(6, 2, argv[i + 1], NULL));
		}
		if (is_redir(argv[i]))
		{
			if (i == argc - 1)
				return (ft_handle_error(5, 2, NULL, NULL));
			if (is_pipe_like(argv[i + 1]) || is_redir(argv[i + 1]))
				return (ft_handle_error(6, 2, argv[i + 1], NULL));
		}
		i++;
	}
	return (0);
}
