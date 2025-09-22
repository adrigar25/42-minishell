/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_dot_or_dotdot.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:29:02 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 14:29:33 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Checks if the given name is "." or "..".
 *
 * SPANISH: Comprueba si el nombre dado es "." o "..".
 *
 * @param name  The name to check. /
 *              El nombre a comprobar.
 *
 * @returns 1 if the name is "." or "..", 0 otherwise. /
 *          1 si el nombre es "." o "..", 0 en caso contrario.
 */
int	ft_is_dot_or_dotdot(const char *name)
{
	return (!ft_strcmp(name, ".") || !ft_strcmp(name, ".."));
}
