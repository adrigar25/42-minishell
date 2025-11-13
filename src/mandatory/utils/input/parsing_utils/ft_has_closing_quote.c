/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_has_closing_quote.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:25:00 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/13 17:25:28 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_has_closing_quote(const char *s, char quote)
{
	while (*s)
	{
		if (*s == quote)
			return (1);
		s++;
	}
	return (0);
}
