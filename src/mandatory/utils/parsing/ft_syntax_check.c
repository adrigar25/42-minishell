/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:25:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 13:34:57 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	is_redir(const char *s)
{
	return (ft_strcmp(s, "<") == 0 || ft_strcmp(s, ">") == 0 || ft_strcmp(s,
			">>") == 0 || ft_strcmp(s, "<<") == 0);
}

static int	is_pipe(const char *s)
{
	return (ft_strcmp(s, "|") == 0);
}

static int	syntax_error(const char *token)
{
	if (!token)
		ft_putstr_error("minishell: syntax error near unexpected token `newline'\n");
	else
	{
		ft_putstr_error("minishell: syntax error near unexpected token `");
		ft_putstr_error((char *)token);
		ft_putstr_error("'\n");
	}
	return (2);
}

int	ft_check_syntax_errors(char **argv, int argc)
{
	int	i;

	if (!argv || argc == 0)
		return (0);
	if (is_pipe(argv[0]))
		return (syntax_error("|"));
	if (is_redir(argv[0]) && argc == 1)
		return (syntax_error("newline"));
	i = 0;
	while (i < argc)
	{
		if (is_pipe(argv[i]))
		{
			if (i == argc - 1)
				return (syntax_error("|"));
			if (is_pipe(argv[i + 1]))
				return (syntax_error("|"));
		}
		if (is_redir(argv[i]))
		{
			if (i == argc - 1)
				return (syntax_error("newline"));
			if (is_pipe(argv[i + 1]) || is_redir(argv[i + 1]))
				return (syntax_error(argv[i + 1]));
		}
		i++;
	}
	return (0);
}
