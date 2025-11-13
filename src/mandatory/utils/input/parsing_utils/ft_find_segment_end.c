/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_find_segment_end.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:04:07 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/13 18:04:13 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_find_segment_end(char **argv, int argc, int start)
{
	int	end;

	end = start;
	while (end < argc && argv[end] && ft_strcmp(argv[end], "|") != 0)
		end++;
	return (end);
}
