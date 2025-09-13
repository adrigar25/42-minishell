/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_msg_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:58:40 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/10 18:00:45 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_msg_start(void)
{
	char	*welcome_msg;

	welcome_msg = ft_search_file(NULL, "welcome.txt");
	ft_putstr("\x1B" "c");
	if (welcome_msg)
	{
		ft_print_file(welcome_msg, COLOR_MAGENTA);
		free(welcome_msg);
	}
	else
		ft_print_file(WELCOME_MSG_TXT, COLOR_MAGENTA);
	ft_putstr("\n");
	ft_putstr(WELCOME_TEXT);
	ft_putstr("\n\n");
	return (0);
}
