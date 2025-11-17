/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_heredoc_line.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 01:30:38 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/17 01:31:06 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_process_heredoc_line(int write_fd, char *line, const char *delimiter,
		t_data *data)
{
	char	*expanded;
	int		has_nl;

	has_nl = (line[ft_strlen(line) - 1] == '\n');
	if (ft_strncmp(line, delimiter, ft_strlen(line) - has_nl) == 0
		&& ft_strlen(line) - has_nl == ft_strlen(delimiter))
	{
		free(line);
		return (1);
	}
	expanded = ft_process_arg(line, data);
	if (!expanded)
		return (-1);
	write(write_fd, expanded, ft_strlen(expanded));
	free(expanded);
	free(line);
	return (0);
}
