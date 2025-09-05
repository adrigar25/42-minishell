/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:18:46 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/05 14:58:15 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Libft library
# include "../libs/libft/libft.h"
# include <dirent.h>

# define WELCOME_MSG_TXT "./src/assets/welcome.txt"

int		ft_minishell(char **envp);
char	*ft_get_directory_path(char *dest);

// Utils
int		ft_is_dot_or_dotdot(const char *name);

// Search for a file in the current directory and subdirectories.
char	*ft_search_file(const char *dir, const char *filename);
char	*ft_search_in_subdirs(const char *dir, const char *filename);
char	*ft_search_in_dir(const char *dir, const char *filename);
char	*ft_build_path(const char *dir, const char *entry);

// Initialize message
int		ft_msg_start(void);

// Count the number of arguments in a command string.
int		ft_count_args(const char *cmd);

// Parsing

char	**ft_parse_input(const char *input, int argc);
void	ft_skip_quotes(const char *cmd, int *i);

// Execution
int		ft_exec_cmd(char **args, int fd_in, int fd_out, char **envp);
char	*get_cmd_path(char *cmd);

// Redirections

void	ft_redir_io(int fd, int in_or_out);

#endif