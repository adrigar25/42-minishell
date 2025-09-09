/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_infile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 20:31:23 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_handle_infile(char *filename)
{
	int	fd;
	char	*error_msg;

	if (!filename)
		return (-1);
	fd = ft_open_file_read(filename);
	if (fd == -1)
	{
		error_msg = ft_strjoin(ft_strjoin("minishell: ", filename), ": No such file or directory\n");
		ft_putstr_error(error_msg);
		free(error_msg);
		return (-1);
	}
	return (fd);
}
