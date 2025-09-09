/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_infile.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 17:13:42 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_handle_infile(char *filename)
{
	int	fd;

	if (!filename)
		return (-1);
	fd = ft_open_file_read(filename);
	if (fd == -1)
	{
		printf("minishell: %s: No such file or directory\n", filename);
		return (-1);
	}
	return (fd);
}
