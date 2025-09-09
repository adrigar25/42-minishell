/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_outfile.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/09 17:13:46 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_handle_outfile(char *filename, int append)
{
	int	fd;
	int	flags;

	if (!filename)
		return (-1);
	fd = ft_open_file_write(filename, append);
	if (fd == -1)
	{
		printf("minishell: %s: Permission denied\n", filename);
		return (-1);
	}
	return (fd);
}
