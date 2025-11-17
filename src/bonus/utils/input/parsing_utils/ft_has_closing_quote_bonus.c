/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_has_closing_quote_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 17:25:00 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/18 00:03:42 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell_bonus.h"

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
