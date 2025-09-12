/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_outfile.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/12 20:09:48 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_handle_outfile(char *filename, int append)
{
	int		fd;
	int		flags;
	char	*error_msg;

	if (!filename)
		return (-1);
	fd = ft_open_file_write(filename, append);
	if (fd == -1)
	{
		error_msg = ft_strjoin(ft_strjoin(ft_strjoin("minishell: ", filename),
					": "), " Permission denied\n");
		ft_putstr_error(error_msg);
		free(error_msg);
		return (-1);
	}
	return (fd);
}
