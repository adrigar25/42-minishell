/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_is_dot_or_dotdot.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/04 18:29:02 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:43:55 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_is_dot_or_dotdot(const char *name)
{
	return (!ft_strcmp(name, ".") || !ft_strcmp(name, ".."));
}
