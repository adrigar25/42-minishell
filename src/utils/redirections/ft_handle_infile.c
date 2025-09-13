/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_infile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/13 15:18:02 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_handle_infile(char *filename)
{
	int		fd;
	char	*error_msg;

	if (!filename)
		return (-1);
	fd = ft_open_file_read(filename);
	if (fd == -1)
	{
		error_msg = ft_strjoin(ft_strjoin("minishell: ", filename),
				": No such file or directory\n");
		ft_putstr_error(error_msg);
		free(error_msg);
		return (-1);
	}
	return (fd);
}
