/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_infile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 16:44:02 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

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
