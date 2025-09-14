/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_close_unused_fds.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42madrid>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2024/12/19 10:30:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell_bonus.h"

void	ft_close_unused_fds(t_cmd *current_cmd, t_cmd *cmd_list) {
  t_cmd *cmd;

  cmd = cmd_list;
  while (cmd) {
    if (cmd != current_cmd) {
      if (cmd->infd != STDIN_FILENO && cmd->infd != current_cmd->infd &&
          cmd->infd != current_cmd->outfd)
        close(cmd->infd);
      if (cmd->outfd != STDOUT_FILENO && cmd->outfd != current_cmd->infd &&
          cmd->outfd != current_cmd->outfd)
        close(cmd->outfd);
    }
    cmd = cmd->next;
  }
}
