/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:18:46 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 18:53:17 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Libft library
# include "../libs/libft/libft.h"

# include <dirent.h>

# define WELCOME_MSG_TXT "./src/assets/welcome.txt"

int			ft_minishell(void);
char		*ft_get_directory_path(char *dest);

// Utils
int			ft_is_dot_or_dotdot(const char *name);

// Search for a file in the current directory and subdirectories.
char		*ft_search_file(const char *dir, const char *filename);
char		*ft_search_in_subdirs(const char *dir, const char *filename);
char		*ft_search_in_dir(const char *dir, const char *filename);
char		*ft_build_path(const char *dir, const char *entry);

// Initialize message
int			ft_msg_start(void);

// Count the number of arguments in a command string.
void		ft_skip_argument(const char *cmd, int *i);
int			ft_count_args(const char *cmd);

#endif