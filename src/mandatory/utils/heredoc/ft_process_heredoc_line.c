/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_heredoc_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:41:00 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/13 17:42:22 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static char	*hdoc_trim_newline(char *line)
{
	size_t	n;

	n = ft_strlen(line);
	if (n > 0 && line[n - 1] == '\n')
		return (ft_substr(line, 0, n - 1));
	return (ft_strdup(line));
}

static int	hdoc_write_expanded_or_original(int write_fd, char *line,
		t_hdoc_ctx *ctx)
{
	char	*expanded;

	if (ctx->expand)
	{
		expanded = NULL;
		if (!ft_process_arg(&expanded, line, ctx->data))
			return (-1);
		if (expanded)
		{
			write(write_fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
	}
	else
		write(write_fd, line, ft_strlen(line));
	return (0);
}

int	ft_process_heredoc_line(int write_fd, char *line, const char *delimiter,
		t_hdoc_ctx *ctx)
{
	char	*cmp;

	cmp = hdoc_trim_newline(line);
	if (!cmp)
	{
		free(line);
		return (-1);
	}
	if (ft_strcmp(cmp, delimiter) == 0)
	{
		free(cmp);
		free(line);
		return (1);
	}
	free(cmp);
	if (hdoc_write_expanded_or_original(write_fd, line, ctx) != 0)
	{
		free(line);
		return (-1);
	}
	free(line);
	return (0);
}
