/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 19:59:15 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/09 01:54:18 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Libft library
# include "../../libs/libft/libft.h"
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

/*
** ===================================================================
**                        MACROS
** ===================================================================
*/

// Exit codes
# define EXIT_SUCCESS 0
# define EXIT_FAILURE 1
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
# define ERROR_COMMAND_NOT_FOUND "minishell: %s: command not found\n"
# define ERROR_PERMISSION_DENIED "minishell: Permission denied\n"
# define ERROR_IS_A_DIRECTORY "minishell: %s: is a directory\n"
# define ERROR_NO_SUCH_FILE "minishell: %s: No such file or directory\n"
# define ERROR_SYNTAX \
	"minishell: \
syntax error near unexpected token `newline'\n"
# define ERROR_SYNTAX_PIPE \
	"minishell: \
syntax error near unexpected token `|'\n"
# define ERROR_SYNTAX_REDIRECT \
	"minishell: \
syntax error near unexpected token `>'\n"
# define ERROR_SYNTAX_TOKEN \
	"minishell: \
syntax error near unexpected token `%s'\n"
# define ERROR_TOO_MANY_ARGS "minishell: too many arguments\n"
# define ERROR_HOME_NOT_SET "minishell: cd: HOME not set\n"
# define ERROR_HEREDOC_DELIMITER "Error: missing delimiter for heredoc\n"
# define ERROR_HEREDOC_PROCESS "Error processing heredoc\n"
# define ERROR_HOME_NOT_SET "minishell: cd: HOME not set\n"
# define ERROR_CD_FAIL "minishell: cd: %s: %s\n"
# define ERROR_AMBIGUOUS_REDIRECT "minishell: %s: ambiguous redirect\n"
# define ERROR_INVALID_IDENTIFIER \
	"minishell:\
 export: `%s': not a valid identifier\n"
# define ERROR_NUM_ARG_REQ "minishell: exit: %s: numeric argument required\n"

// Heredoc
# define HEREDOC_PROMPT "heredoc> "

/*
** ===================================================================
**                        DATA STRUCTURES
** ===================================================================
*/

/**
 * ENGLISH: Represents the main structure for managing the shell state
 * 	in the minishell project.
 *
 * - Holds all relevant data for environment management, argument count,
 * 	command tracking, and shell status.
 * - Centralizes pointers to environment variables and links to
 * 	other shell data nodes.
 * - Facilitates access and modification of shell state throughout the program.
 *
 * SPANISH: Representa la estructura principal para gestionar el estado del
 * 	shell en el proyecto minishell.
 *
 * - Contiene todos los datos relevantes para la gestión del entorno,
 * 	el recuento de argumentos, el seguimiento de comandos y el estado del shell.
 * - Centraliza los punteros a las variables de entorno y enlaza con otros
 * 	nodos de datos del shell.
 * - Facilita el acceso y la modificación del estado del shell en
 * 	todo el programa.
 *
 * ENGLISH/SPANISH: Variable documentation for t_data structure.
 *
 * @member {char **} envp
 *      - EN: Array of environment variable strings.
 *      - ES: Array de cadenas de variables de entorno.
 *
 * @member {int} argc
 *      - EN: Number of arguments passed to the shell.
 *      - ES: Número de argumentos pasados al shell.
 *
 * @member {int} cmd_count
 *      - EN: Number of commands processed or to be processed.
 *      - ES: Número de comandos procesados o a procesar.
 *
 * @member {int} isatty
 *      - EN: Indicates if the shell is running in a terminal (1 = yes, 0 = no).
 *      - ES: Indica si el shell se ejecuta en un terminal (1 = sí, 0 = no).
 *
 * @member {int} last_exit_status
 *      - EN: Exit status of the last executed command.
 *      - ES: Estado de salida del último comando ejecutado.
 *
 * @member {struct s_data *} next
 *      - EN: Pointer to the next shell data node (for chaining or history).
 *      - ES: Puntero al siguiente nodo de datos del shell (para encadenar o
 * 			historial).
 */
typedef struct s_data
{
	char			**envp;
	int				argc;
	int				cmd_count;
	int				isatty;
	int				last_exit_status;
	struct s_data	*next;
}					t_data;

/**
 * ENGLISH: Represents a command node in the minishell command list.
 *
 * - Holds command arguments, input/output file descriptors, and error state.
 * - Links to shell data and the next command in a pipeline or sequence.
 * - Used for parsing, execution, and management of shell commands.
 *
 * SPANISH: Representa un nodo de comando en la lista de comandos de minishell.
 *
 * - Contiene los argumentos del comando, descriptores de archivos de
 * 	entrada/salida y estado de error.
 * - Enlaza con los datos del shell y el siguiente comando en una tubería
 * 	o secuencia.
 * - Se utiliza para el análisis, ejecución y gestión de los comandos del shell.
 *
 * ENGLISH/SPANISH: Variable documentation for t_cmd structure.
 *
 * @member {char **} argv
 *      - EN: Array of command arguments.
 *      - ES: Array de argumentos del comando.
 *
 * @member {int} infd
 *      - EN: Input file descriptor for the command.
 *      - ES: Descriptor de archivo de entrada para el comando.
 *
 * @member {int} outfd
 *      - EN: Output file descriptor for the command.
 *      - ES: Descriptor de archivo de salida para el comando.
 *
 * @member {int} has_error
 *      - EN: Indicates if the command has an error (1 = error, 0 = no error).
 *      - ES: Indica si el comando tiene un error (1 = error, 0 = sin error).
 *
 * @member {int} index
 *      - EN: Index of the command in the pipeline or sequence.
 *      - ES: Índice del comando en la tubería o secuencia.
 *
 * @member {t_data *} data
 *      - EN: Pointer to the shell's main data structure.
 *      - ES: Puntero a la estructura principal de datos del shell.
 *
 * @member {struct s_cmd *} next
 *      - EN: Pointer to the next command node.
 *      - ES: Puntero al siguiente nodo de comando.
 */
typedef struct s_cmd
{
	char			**argv;
	int				infd;
	int				outfd;
	int				has_error;
	int				index;
	t_data			*data;
	struct s_cmd	*next;
}					t_cmd;

/*
** ===================================================================
**                        FUNCTIONS
** ===================================================================
*/

int					ft_minishell(char **envp, int debug);

// Search for a file in the current directory and subdirectories.
char				*ft_search_file(const char *dir, const char *filename);
char				*ft_search_in_subdirs(const char *dir,
						const char *filename);
char				*ft_search_in_dir(const char *dir, const char *filename);
char				*ft_build_path(const char *dir, const char *entry);

// Expansion
int					ft_copy_literal(char **dst, char *arg, int start, int end);
int					ft_expand_exit_status(char **dst, int *j, t_data *data);
int					ft_expand_env_var(char **dst, char *arg, int *j,
						t_data *data);
int					ft_process_arg(char **dst, char *arg, t_data *data);
char				**ft_handle_env_expansion(char **argv, t_data *data);

// Execution
char				*ft_get_cmd_path(char *cmd);
int					ft_finish_execution(pid_t *pids, t_cmd *cmd_list,
						t_data *data);
void				ft_setup_child_io(t_cmd *current, t_cmd *cmd_list);
int					ft_exec_builtin(t_cmd *cmd, t_data **data);
int					ft_is_builtin(t_cmd *cmd);
int					ft_should_execute(t_cmd **current, t_data *data);
int					ft_execute_cmds(t_cmd *cmd_list, t_data **data);
int					ft_exec_bin(t_cmd *cmd);

// PROMPT
char				*ft_get_directory_path(char **envp);
char				*ft_generate_prompt(char **envp);

// Initialize message
int					ft_msg_start(void);

// Count the number of arguments in a command string.
int					ft_count_args(const char *cmd);

// Memory management utilities
void				ft_free_matrix(char **array);
void				ft_free_matrix_size(char **array, int size);

// INPUT

int					ft_read_input(char **input, t_data *data);
char				**ft_split_input(const char *input, int argc);
t_cmd				*ft_process_input(char *input, t_data *data, int debug);
int					ft_check_input_syntax(char **argv, int argc);
char				*ft_remove_quotes(const char *str);

// Parsing

t_cmd				*ft_parse_input(char **argv, t_data *data);
int					ft_append(char **dst, const char *src);
t_cmd				*ft_create_cmd_node(int index);
void				ft_add_arg_to_cmd(t_cmd *cmd, char *arg);
int					ft_assign_fd(t_cmd **cmd, char *filename, char *type);
int					ft_redir(t_cmd *cmd, char **argv, int i);
int					ft_process_pipe(t_cmd **current_cmd, int *cmd_index,
						t_data *data);
int					ft_process_token(t_cmd **current_cmd, char **argv, int i,
						int *cmd_index);

// ENV

char				*ft_create_env_var(char *name, char *value);
char				**ft_dupenv(char **envp);
char				*ft_getenv(const char *name, char **envp);
char				**ft_realloc_envp(char **envp, int new_size);
int					ft_setenv(char *name, char *value, char ***envp);
int					ft_update_existing_env(char *name, char *value,
						char **envp);
void				ft_update_pwd_env(char *oldpwd, char *target_dir,
						char ***envp);

// DEBUG

void				ft_show_debug(char **argv, int argc, char **expanded_argv,
						t_cmd *cmd_list);

// Redirections
int					ft_handle_infile(char *filename);
int					ft_handle_outfile(char *filename, int append);

// Signals
void				sigint_handler(int sig);
void				ft_init_signals(void);

// Builtins
int					ft_echo(t_cmd cmd);
int					ft_cd(char **argv, char ***envp);
int					ft_pwd(t_cmd cmd);
int					ft_export(char **args, char ***envp);
int					ft_unset(char **args, char ***envp);
int					ft_env(t_cmd cmd, char **envp);
int					ft_exit(t_cmd *cmd);
int					ft_heredoc(const char *delimiter, t_data *data, int expand);

// Utils
int					ft_is_dot_or_dotdot(const char *name);

// Heredoc
// int					ft_heredoc(const char *delimiter);
// Error handling
int					ft_handle_error(int error_code, int exit_code, char *msg,
						char *msg2);
#endif
