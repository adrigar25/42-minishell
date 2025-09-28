/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_append_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 00:51:17 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 22:27:16 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

/**
 * Appends src string to dst string,
 * reallocating dst as needed.
 *
 * @param dst Pointer to the destination string
 * (will be freed and replaced).
 * @param src Source string to append.
 * @returns 1 on success, 0 on failure.
 */
int	ft_append(char **dst, const char *src)
{
	char	*temp;

	if (!dst || !src)
		return (0);
	temp = *dst;
	*dst = ft_strjoin(temp, (char *)src);
	free(temp);
	if (!*dst)
		return (0);
	return (1);
}
