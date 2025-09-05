/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_outfile.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 19:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 19:13:11 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	ft_handle_outfile(char *filename, int append)
{
	int	fd;
	int	flags;

	if (!filename)
		return (-1);
	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		printf("minishell: %s: Permission denied\n", filename);
		return (-1);
	}
	return (fd);
}
