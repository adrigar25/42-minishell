/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 14:20:19 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/12 20:07:15 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_escaped(const char *cmd, int pos, int in_quote, char quote_char)
{
	int	backslashes;
	int	j;

	backslashes = 0;
	j = pos - 1;
	if (in_quote && quote_char == '\'')
		return (0);
	while (j >= 0 && cmd[j] == '\\')
	{
		backslashes++;
		j--;
	}
	return (backslashes % 2 == 1);
}

static void	skip_quotes_and_escapes(const char *cmd, int *i, int *in_quote,
		char *quote_char)
{
	while (cmd[*i] && ((*in_quote) || (!ft_isspace(cmd[*i]) && cmd[*i] != '|'
				&& cmd[*i] != '<' && cmd[*i] != '>')))
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

static int	handle_operator(const char *cmd, int *i, int in_quote,
		char quote_char)
{
	if ((cmd[*i] == '<' || cmd[*i] == '>') && cmd[*i + 1] == cmd[*i]
		&& !is_escaped(cmd, *i, in_quote, quote_char))
	{
		(*i) += 2;
		ft_skip_whitespace(cmd, i);
		return (1);
	}
	else if ((cmd[*i] == '<' || cmd[*i] == '>') && !is_escaped(cmd, *i,
			in_quote, quote_char))
	{
		(*i)++;
		ft_skip_whitespace(cmd, i);
		return (1);
	}
	else if (cmd[*i] == '|' && !is_escaped(cmd, *i, in_quote, quote_char))
	{
		(*i)++;
		while (cmd[*i] == '|' && !is_escaped(cmd, *i, in_quote, quote_char))
			(*i)++;
		return (1);
	}
	return (0);
}

static int	count_word(const char *cmd, int *i)
{
	int		count;
	int		in_quote;
	char	quote_char;
	int		start;

	count = 0;
	in_quote = 0;
	quote_char = 0;
	start = *i;
	skip_quotes_and_escapes(cmd, i, &in_quote, &quote_char);
	if (*i > start)
		count = 1;
	if (*i == start)
		(*i)++;
	return (count);
}

int	ft_count_args(const char *cmd)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (cmd[i])
	{
		ft_skip_whitespace(cmd, &i);
		if (!cmd[i])
			break ;
		if (handle_operator(cmd, &i, 0, 0))
			count++;
		else
			count += count_word(cmd, &i);
	}
	return (count);
}
