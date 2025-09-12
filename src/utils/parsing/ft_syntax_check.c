/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_syntax_check.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:25:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 23:15:14 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_is_quoted_token(const char *token)
{
	int	len;

	if (!token)
		return (0);
	len = ft_strlen(token);
	if (len < 2)
		return (0);
	if ((token[0] == '"' && token[len - 1] == '"') || (token[0] == '\''
			&& token[len - 1] == '\''))
		return (1);
	return (0);
}

static int	ft_check_redirection_syntax(char **argv, int i, int argc)
{
	char	*error_msg;
	char	*temp;

	if ((ft_strcmp(argv[i], ">") == 0 || ft_strcmp(argv[i], ">>") == 0
			|| ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], "<<") == 0)
		&& !ft_is_quoted_token(argv[i]) && i + 1 >= argc)
	{
		ft_putstr_error(ERROR_SYNTAX);
		return (1);
	}
	if ((ft_strcmp(argv[i], ">") == 0 || ft_strcmp(argv[i], ">>") == 0
			|| ft_strcmp(argv[i], "<") == 0 || ft_strcmp(argv[i], "<<") == 0)
		&& !ft_is_quoted_token(argv[i]) && i + 1 < argc)
	{
		if ((ft_strcmp(argv[i + 1], "|") == 0 || ft_strcmp(argv[i + 1],
					">") == 0 || ft_strcmp(argv[i + 1], ">>") == 0
				|| ft_strcmp(argv[i + 1], "<") == 0 || ft_strcmp(argv[i + 1],
					"<<") == 0) && !ft_is_quoted_token(argv[i + 1]))
		{
			temp = ft_strjoin(argv[i + 1], "'\n");
			error_msg = ft_strjoin(ERROR_SYNTAX_TOKEN, temp);
			ft_putstr_error(error_msg);
			free(temp);
			free(error_msg);
			return (1);
		}
	}
	return (0);
}

static int	ft_check_pipe_syntax(char **argv, int i, int argc)
{
	if ((ft_strcmp(argv[i], "|") == 0) && !ft_is_quoted_token(argv[i]))
	{
		if (i == 0 || i == argc - 1)
		{
			ft_putstr_error(ERROR_SYNTAX_PIPE);
			return (1);
		}
		if (i + 1 < argc && (ft_strcmp(argv[i + 1], "|") == 0)
			&& !ft_is_quoted_token(argv[i + 1]))
		{
			ft_putstr_error(ERROR_SYNTAX_PIPE);
			return (1);
		}
		if (i + 1 < argc && ((ft_strcmp(argv[i + 1], ">") == 0
					|| ft_strcmp(argv[i + 1], ">>") == 0)
				&& !ft_is_quoted_token(argv[i + 1])))
		{
			ft_putstr_error(ERROR_SYNTAX);
			return (1);
		}
	}
	return (0);
}

static int	ft_check_special_redirect(char **argv, int i, int argc)
{
	if ((ft_strcmp(argv[i], ">>") == 0) && !ft_is_quoted_token(argv[i]) && i
		+ 1 < argc && (ft_strcmp(argv[i + 1], ">") == 0)
		&& !ft_is_quoted_token(argv[i + 1]))
	{
		ft_putstr_error(ERROR_SYNTAX_REDIRECT);
		return (1);
	}
	return (0);
}

int	ft_check_syntax_errors(char **argv, int argc)
{
	int	i;

	if (!argv || argc == 0)
		return (0);
	if (ft_strcmp(argv[0], "|") == 0 && !ft_is_quoted_token(argv[0]))
	{
		ft_putstr_error(ERROR_SYNTAX_PIPE);
		return (1);
	}
	if ((ft_strcmp(argv[0], ">") == 0 || ft_strcmp(argv[0], ">>") == 0
			|| ft_strcmp(argv[0], "<") == 0 || ft_strcmp(argv[0], "<<") == 0)
		&& !ft_is_quoted_token(argv[0]))
	{
		ft_putstr_error(ERROR_SYNTAX);
		return (1);
	}
	i = 0;
	while (i < argc)
	{
		if (ft_check_redirection_syntax(argv, i, argc))
			return (1);
		if (ft_check_pipe_syntax(argv, i, argc))
			return (1);
		if (ft_check_special_redirect(argv, i, argc))
			return (1);
		i++;
	}
	return (0);
}
