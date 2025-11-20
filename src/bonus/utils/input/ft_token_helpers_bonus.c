/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_token_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auto <auto@local>                           +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 22:20:00 by auto              #+#    #+#             */
/*   Updated: 2025/11/16 22:20:00 by auto             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/*
 * Skip over quoted sections and escaped characters.
 * Used by argument counting and token parsing.
 */
void	ft_skip_quotes_and_escapes(const char *cmd, int *i, int *in_quote,
		char *quote_char)
{
	while (cmd[*i] && ((*in_quote) || (!ft_isspace(cmd[*i]) && cmd[*i] != '|'
				&& cmd[*i] != '<' && cmd[*i] != '>' && cmd[*i] != '&')))
	{
		if (!(*in_quote) && (cmd[*i] == '\'' || cmd[*i] == '"'))
		{
			*in_quote = 1;
			*quote_char = cmd[*i];
			(*i)++;
		}
		else if (*in_quote && cmd[*i] == *quote_char)
		{
			*in_quote = 0;
			*quote_char = 0;
			(*i)++;
		}
		else if (cmd[*i] == '\\' && cmd[*i + 1])
			(*i) += 2;
		else
			(*i)++;
	}
}

/*
 * Handle operators like <, >, >>, |, &&, || in the command string.
 * Returns 1 if an operator was handled (and index moved), 0 otherwise.
 */
int	ft_handle_operator(const char *cmd, int *i)
{
	int	escaped;

	escaped = ft_is_escaped(cmd, *i);
	if ((cmd[*i] == '<' || cmd[*i] == '>') && cmd[*i + 1] == cmd[*i]
		&& !escaped)
	{
		(*i) += 2;
		ft_skip_whitespace(cmd, i);
		return (1);
	}
	else if ((cmd[*i] == '<' || cmd[*i] == '>') && !escaped)
	{
		(*i)++;
		ft_skip_whitespace(cmd, i);
		return (1);
	}
	else if ((cmd[*i] == '|' || cmd[*i] == '&') && !escaped)
	{
		if (cmd[*i + 1] == cmd[*i] && !ft_is_escaped(cmd, *i + 1))
			(*i) += 2;
		else
			(*i)++;
		return (1);
	}
	return (0);
}
