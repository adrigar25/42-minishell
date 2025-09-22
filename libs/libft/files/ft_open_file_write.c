/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_open_file_write.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 14:23:10 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 12:48:49 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/**
 * ENGLISH: Opens a file for writing.
 *
 * SPANISH: Abre un archivo para escritura.
 *
 * @param filename   The name of the file to open. /
 *                   El nombre del archivo a abrir.
 *
 * @returns The file descriptor for the opened file, or -1 on failure. /
 *          El descriptor de archivo para el archivo abierto, o -1
 * 			en caso de error.
 */
int	ft_open_file_write(const char *filename, int append)
{
	int	fd;
	int	flags;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd < 0)
		return (-1);
	return (fd);
}
