/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:18:46 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:05:22 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Libft library
#include "../libs/libft/libft.h"

#include <dirent.h>

# define WELCOME_MSG_TXT "./src/assets/welcome.txt"

int			ft_minishell(void);
char 		*ft_get_directory_path(char *dest);
// Initialize message
int			ft_msg_start(void);

// Count the number of arguments in a command string.
void		ft_skip_argument(const char *cmd, int *i);
int			ft_count_args(const char *cmd);

#endif