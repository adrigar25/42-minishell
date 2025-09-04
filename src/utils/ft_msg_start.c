/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_msg_start.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:58:40 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 17:21:52 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_msg_start(void)
{
	system("clear");
	ft_print_file(WELCOME_MSG_TXT, "\033[0;32m");
	ft_putstr("\n");
	ft_putstr("\033[0;34mWelcome to Minishell!\033[0m\n");
	ft_putstr("\n\n");
	return (0);
}
