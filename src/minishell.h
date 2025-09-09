/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:18:46 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/09 18:32:12 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Libft library
# include "../libs/libft/libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>

/*
** ============================================================================
**                                 MACROS
** ============================================================================
*/

// Exit codes
# define EXIT_SUCCESS 0
# define EXIT_GENERAL_ERROR 1
# define EXIT_MISUSE 2
# define EXIT_COMMAND_NOT_FOUND 127
# define EXIT_PERMISSION_DENIED 126

// File descriptors
# define STDIN_FD 0
# define STDOUT_FD 1
# define STDERR_FD 2

// Prompt and messages
# define SHELL_NAME "minishell"
# define PROMPT_PREFIX "\033[90mminishell:("
# define PROMPT_SUFFIX ")\033[0m> "
# define WELCOME_MSG_TXT "./src/assets/welcome.txt"
# define WELCOME_TEXT "\033[0;34mWelcome to Minishell!\033[0m\n"

// Colors
# define COLOR_RESET "\033[0m"
# define COLOR_RED "\033[0;31m"
# define COLOR_GREEN "\033[0;32m"
# define COLOR_BLUE "\033[0;34m"
# define COLOR_GRAY "\033[90m"
# define COLOR_YELLOW "\033[0;33m"
# define COLOR_CYAN "\033[0;36m"
# define COLOR_MAGENTA "\033[0;35m"

// Error messages
# define ERROR_COMMAND_NOT_FOUND "minishell: command not found: %s\n"
# define ERROR_PERMISSION_DENIED "minishell: %s: Permission denied\n"
# define ERROR_NO_SUCH_FILE "minishell: %s: No such file or directory\n"
# define ERROR_SYNTAX "minishell: syntax error near unexpected token `newline'\n"
# define ERROR_TOO_MANY_ARGS "minishell: %s: too many arguments\n"
# define ERROR_HOME_NOT_SET "minishell: cd: HOME not set\n"
# define ERROR_HEREDOC_DELIMITER "Error: missing delimiter for heredoc\n"
# define ERROR_HEREDOC_PROCESS "Error processing heredoc\n"

// Heredoc
# define HEREDOC_PROMPT "heredoc> "

typedef struct s_cmd
{
	char			**argv;
	int				infd;
	int				outfd;
	struct s_cmd	*next;
}					t_cmd;

int					ft_minishell(char **envp);
int					ft_save_envp(char ***envp_cpy, char **envp);
char				*ft_get_directory_path(char *dest);

// Search for a file in the current directory and subdirectories.
char				*ft_search_file(const char *dir, const char *filename);
char				*ft_search_in_subdirs(const char *dir,
						const char *filename);
char				*ft_search_in_dir(const char *dir, const char *filename);
char				*ft_build_path(const char *dir, const char *entry);

// Parsing

char				**ft_split_input(const char *input, int argc);
t_cmd				*ft_parse_input(char **argv, int argc);
void				ft_skip_quotes(const char *cmd, int *i);
char				*ft_trim(const char *str, char c);

// Execution
int					ft_exec_cmd(t_cmd *cmd, char **envp);
int					ft_pipex(const char **argv, int fd_in, char **envp);
char				*get_cmd_path(char *cmd);

/********** */
/*  Utils   */
/********** */

// Initialize message
int					ft_msg_start(void);
char				*ft_generate_prompt(void);

// Count the number of arguments in a command string.
int					ft_count_args(const char *cmd);

// Split strings by separator
char				***ft_split_strings(const char **argv, const char *sep);
void				free_split_strings(char ***cmds);

// Redirections
void				ft_redir_io(int fd, int in_or_out);
int					ft_handle_infile(char *filename);
int					ft_handle_outfile(char *filename, int append);

// Signals
void				sigint_handler(int sig);
void				ft_init_signals(void);

// Builtins
int					ft_echo(t_cmd cmd);
int					ft_cd(char **args, char **envp);
int					ft_pwd(char **args);
int					ft_export(char **args, char ***envp);
int					ft_unset(char **args, char ***envp);
int					ft_env(char **envp);
int					ft_exit(char **args);
int					ft_handle_builtins(t_cmd *cmd, char ***envp);

int					ft_is_dot_or_dotdot(const char *name);

#endif
