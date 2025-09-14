/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_infile_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/14 14:34:15 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 22:54:47 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

int	ft_handle_infile(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = ft_open_file_read(filename);
	if (fd == -1)
		return (ft_handle_error(4, -1, filename, NULL));
	return (fd);
}
