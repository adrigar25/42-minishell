/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_search_file.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:22:57 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:50:06 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_search_file(const char *dir, const char *filename)
{
	const char	*start_dir;

	if (dir)
		start_dir = dir;
	else
		start_dir = getenv("PWD");
	return (ft_search_in_dir(start_dir, filename));
}
