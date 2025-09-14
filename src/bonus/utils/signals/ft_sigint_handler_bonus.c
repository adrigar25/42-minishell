/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sigint_handler_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 18:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/14 15:18:12 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

void	sigint_handler(int sig) {
  (void)sig;
  write(1, "\n", 1);
  rl_on_new_line();
  rl_replace_line("", 0);
  rl_redisplay();
}
