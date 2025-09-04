/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_build_path.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:35:06 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:35:18 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_build_path(const char *dir, const char *entry)
{
	char	path[4096];

	snprintf(path, sizeof(path), "%s/%s", dir, entry);
	return (ft_strdup(path));
}
