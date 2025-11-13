/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_skip_nulls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:01:02 by adriescr          #+#    #+#             */
/*   Updated: 2025/11/13 18:02:59 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../minishell.h"

int	ft_skip_nulls(char **argv, int argc, int i)
{
	while (i < argc && argv[i] == NULL)
		i++;
	return (i);
}
