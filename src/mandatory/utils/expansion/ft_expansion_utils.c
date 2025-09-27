/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expansion_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/27 19:05:41 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_copy_literal(char **dst, char *arg, int start, int end)
{
	char	*literal;
	char	*temp;

	if (end <= start)
		return (1);
	literal = ft_substr(arg, start, end - start);
	if (!literal)
		return (0);
	temp = *dst;
	*dst = ft_strjoin(temp, literal);
	free(temp);
	free(literal);
	if (!*dst)
		return (0);
	return (1);
}
