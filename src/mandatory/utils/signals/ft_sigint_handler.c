/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sigint_handler.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/10/28 16:48:24 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Handles the SIGINT signal (Ctrl+C) for the shell.
 *
 * SPANISH: Maneja la señal SIGINT (Ctrl+C) para el shell.
 *
 * @param sig   The signal number. /
 *              El número de señal.
 */
void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}
