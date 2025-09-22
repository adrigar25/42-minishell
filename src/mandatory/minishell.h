/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:22:24 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 16:10:24 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// Libft library
# include "../../libs/libft/libft.h"

// Library dependencies
# include <dirent.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>

/*
** ===================================================================
**                             MACROS
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

/*
** ===================================================================
**                        ERROR MESSAGES
** ===================================================================
*/

/**
 * @def ERROR_COMMAND_NOT_FOUND
 *
 * ENGLISH:
 *
 * @brief Error message for command not found in minishell.
 *
 * This macro defines the error message displayed when a command
 * is not found in the system. The '%s' placeholder should be
 * replaced with the name of the command that was not found.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 * 	printf(ERROR_COMMAND_NOT_FOUND, cmd_name);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para comando no encontrado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando un comando
 * no se encuentra en el sistema. El marcador '%s' debe ser reemplazado
 * por el nombre del comando que no se encontró. El mensaje se imprime
 * para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * 	printf(ERROR_COMMAND_NOT_FOUND, cmd_name);
 */
# define ERROR_COMMAND_NOT_FOUND "minishell: %s: command not found\n"

/**
 * @def ERROR_PERMISSION_DENIED
 *
 * ENGLISH:
 *
 * @brief Error message for permission denied in minishell.
 *
 * This macro defines the error message displayed when a command
 * cannot be executed due to insufficient permissions.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 * 	printf(ERROR_PERMISSION_DENIED);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para permiso denegado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando un comando
 * no puede ser ejecutado debido a permisos insuficientes.
 * El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * 	printf(ERROR_PERMISSION_DENIED);
 */
# define ERROR_PERMISSION_DENIED "minishell: Permission denied\n"

/**
 * @def ERROR_IS_A_DIRECTORY
 *
 * ENGLISH:
 *
 * @brief Error message for "is a directory" in minishell.
 *
 * This macro defines the error message displayed when a command
 * attempts to execute a directory instead of a file. The '%s' placeholder
 * should be replaced with the name of the directory that was incorrectly
 * treated as a command. The message is printed to inform the user of the issue.
 *
 * Example usage:
 * 	printf(ERROR_IS_A_DIRECTORY, dirname);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para "es un directorio" en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando un comando
 * intenta ejecutar un directorio en lugar de un archivo. El marcador '%s'
 * debe ser reemplazado por el nombre del directorio que fue tratado
 * incorrectamente como un comando.
 * El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * 	printf(ERROR_IS_A_DIRECTORY, dirname);
 */
# define ERROR_IS_A_DIRECTORY "minishell: %s: is a directory\n"

/**
 * @def ERROR_NO_SUCH_FILE
 *
 * ENGLISH:
 *
 * @brief Error message for file or directory not found in minishell.
 *
 * This macro defines the error message displayed when a specified file or
 * directory cannot be found in the filesystem. The '%s' placeholder should
 * be replaced with the name of the file or directory that was not found.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 * printf(ERROR_NO_SUCH_FILE, filename);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para archivo o directorio no encontrado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando no se puede
 * encontrar un archivo o directorio especificado en el sistema de archivos.
 * El marcador '%s' debe ser reemplazado por el nombre del archivo o directorio
 * que no se encontró.
 * El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * printf(ERROR_NO_SUCH_FILE, filename);
 */
# define ERROR_NO_SUCH_FILE "minishell: %s: No such file or directory\n"

/**
 * @def ERROR_SYNTAX
 *
 * ENGLISH:
 *
 * @brief Error message for unexpected newline token in minishell.
 *
 * This macro defines the error message displayed when a syntax error occurs
 * due to an unexpected newline token in the command input.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 * printf(ERROR_SYNTAX);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para token de nueva línea inesperado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando ocurre un error
 * de sintaxis debido a un token de nueva línea inesperado en la entrada del
 * comando.
 * El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * printf(ERROR_SYNTAX);
 */
# define ERROR_SYNTAX \
 "minishell: syntax error near unexpected token `newline'\n"

/**
 * @def ERROR_SYNTAX_PIPE
 *
 * ENGLISH:
 *
 * @brief Error message for unexpected pipe token in minishell.
 *
 * This macro defines the error message displayed when a syntax error occurs
 * due to an unexpected pipe token in the command input.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 * printf(ERROR_SYNTAX_PIPE);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para token de tubería inesperado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando ocurre un
 * error de sintaxis debido a un token de tubería inesperado en la entrada
 * del comando. El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * printf(ERROR_SYNTAX_PIPE);
 */
# define ERROR_SYNTAX_PIPE "minishell: syntax error near unexpected token `|'\n"

/**
 * @def ERROR_SYNTAX_REDIRECT
 *
 * ENGLISH:
 *
 * @brief Error message for unexpected redirection token in minishell.
 *
 * This macro defines the error message displayed when a syntax error occurs
 * due to an unexpected redirection token in the command input.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 *  printf(ERROR_SYNTAX_REDIRECT);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para token de redirección inesperado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando ocurre
 * un error de sintaxis debido a un token de redirección inesperado en la
 * entrada del comando.
 * El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * printf(ERROR_SYNTAX_REDIRECT);
 */
# define ERROR_SYNTAX_REDIRECT \
 "minishell: syntax error near unexpected token `>'\n"

/**
 * @def ERROR_SYNTAX_TOKEN
 *
 * ENGLISH:
 *
 * @brief Error message format for unexpected token syntax error in minishell.
 *
 * This macro defines the error message displayed when a syntax error occurs
 * due to an unexpected token in the command input. The '%s' placeholder
 * should be replaced with the specific token that caused the error.
 *
 * Example usage:
 *   printf(ERROR_SYNTAX_TOKEN, token);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para error de sintaxis por token
 * inesperado en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando ocurre un
 * error de sintaxis debido a un token inesperado en la entrada del comando.
 * El marcador '%s' debe ser reemplazado por el token específico que causó
 * el error.
 *
 * Ejemplo de uso:
 *  printf(ERROR_SYNTAX_TOKEN, token);
 */
# define ERROR_SYNTAX_TOKEN \
 "minishell: syntax error near unexpected token `%s'\n"

/**
 * @def ERROR_TOO_MANY_ARGS
 *
 * ENGLISH:
 *
 * @brief Error message for too many arguments in minishell.
 *
 * This macro defines the error message displayed when a command in minishell
 * receives more arguments than it can handle. This situation typically occurs
 * when a user provides an excessive number of arguments to a command,
 * leading to confusion or unexpected behavior. The message is printed to
 * inform the user of the issue.
 *
 * Example usage:
 *  printf(ERROR_TOO_MANY_ARGS);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para demasiados argumentos en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando un comando en
 * minishell recibe más argumentos de los que puede manejar. Esta situación
 * suele ocurrir cuando un usuario proporciona un número excesivo de
 * argumentos a un comando, lo que puede llevar a confusión o comportamiento
 * inesperado. El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 * printf(ERROR_TOO_MANY_ARGS);
 */
# define ERROR_TOO_MANY_ARGS "minishell: too many arguments\n"

/**
 * @def ERROR_HOME_NOT_SET
 *
 * ENGLISH:
 *
 * @brief Error message for unset HOME variable in minishell.
 *
 * This macro defines the error message displayed when the 'cd' command
 * in minishell is executed but the HOME environment variable is not set.
 * This situation typically occurs when a user attempts to change
 * to their home directory without the HOME variable being defined.
 * The message is printed to inform the user of the issue.
 *
 * Example usage:
 *   printf(ERROR_HOME_NOT_SET);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para variable HOME no establecida en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando se ejecuta
 * el comando 'cd' en minishell pero la variable de entorno HOME no está
 * establecida.
 * Esta situación suele ocurrir cuando un usuario intenta cambiar
 * a su directorio home sin que la variable HOME esté definida.
 * El mensaje se imprime para informar al usuario del problema.
 *
 * Ejemplo de uso:
 *  printf(ERROR_HOME_NOT_SET);
 */
# define ERROR_HOME_NOT_SET "minishell: cd: HOME not set\n"

/**
 * @def ERROR_CD_FAIL
 *
 * ENGLISH:
 *
 * @brief Error message format for 'cd' command failure in minishell.
 *
 * This macro defines the error message displayed when the 'cd' command in
 * minishell fails to change the directory. The first '%s' placeholder
 * should be replaced with the target directory, and the second '%s' with
 * the specific error message.
 *
 * Example usage:
 *  printf(ERROR_CD_FAIL, target_dir, strerror(errno));
 *
 * SPANISH:
 *
 * @brief Mensaje de error para fallo del comando 'cd' en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando el comando
 * 'cd' en minishell no puede cambiar el directorio. El primer marcador '%s'
 * debe ser reemplazado por el directorio objetivo, y el segundo '%s' por el
 * mensaje de error específico.
 *
 * Ejemplo de uso:
 * printf(ERROR_CD_FAIL, target_dir, strerror(errno));
 */
# define ERROR_HOME_NOT_SET "minishell: cd: HOME not set\n"

/**
 * @def ERROR_CD_FAIL
 *
 * ENGLISH:
 *
 * @brief Error message format for 'cd' command failure in minishell.
 *
 * This macro defines the error message displayed when the 'cd' command in
 * minishell fails to change the directory. The first '%s' placeholder
 * should be replaced with the target directory, and the second '%s'
 * with the specific error message.
 *
 * Example usage:
 *  printf(ERROR_CD_FAIL, target_dir, strerror(errno));
 *
 * SPANISH:
 *
 * @brief Mensaje de error para fallo del comando 'cd' en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando el comando 'cd'
 * en minishell no puede cambiar el directorio. El primer marcador '%s'
 * debe ser reemplazado por el directorio objetivo, y el segundo '%s' por el
 * mensaje de error específico.
 *
 * Ejemplo de uso:
 * printf(ERROR_CD_FAIL, target_dir, strerror(errno));
 */
# define ERROR_CD_FAIL "minishell: cd: %s: %s\n"

/**
 * @def ERROR_AMBIGUOUS_REDIRECT
 *
 * ENGLISH:
 *
 * @brief Error message format for ambiguous redirect in minishell.
 *
 * This macro defines the error message displayed when a command in minishell
 * encounters an ambiguous redirect situation. The '%s' placeholder should be
 * replaced with the relevant filename or token causing the ambiguity.
 *
 * Example usage:
 *   printf(ERROR_AMBIGUOUS_REDIRECT, filename);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para redirección ambigua en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando un
 * comando en minishell
 * encuentra una situación de redirección ambigua. El marcador '%s' debe ser
 * reemplazado por el nombre de archivo o token relevante que causa la
 * ambigüedad.
 *
 * Ejemplo de uso:
 *   printf(ERROR_AMBIGUOUS_REDIRECT, filename);
 */
# define ERROR_AMBIGUOUS_REDIRECT "minishell: %s: ambiguous redirect\n"

/**
 * @def ERROR_INVALID_IDENTIFIER
 *
 * ENGLISH:
 *
 * @brief Error message format for invalid identifier in the 'export' command.
 *
 * This macro defines the error message displayed when the 'export'
 * command in minishell
 * receives an argument that is not a valid identifier.
 * The '%s' placeholder should be replaced with the invalid identifier.
 *
 * Example usage:
 *   printf(ERROR_INVALID_IDENTIFIER, identifier);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para identificador no válido en el comando 'export'.
 *
 * Este macro define el mensaje de error que se muestra cuando el
 * comando 'export' en minishell
 * recibe un argumento que no es un identificador válido.
 * El marcador '%s' debe ser reemplazado por el identificador inválido.
 *
 * Ejemplo de uso:
 *   printf(ERROR_INVALID_IDENTIFIER, identifier);
 */
# define ERROR_INVALID_IDENTIFIER \
	"minishell: export: `%s': not a valid identifier\n"

/**
* @def ERROR_NUM_ARG_REQ
*
* ENGLISH:
*
* @brief Error message format for non-numeric argument in the 'exit' command.
*
* This macro defines the error message displayed when the 'exit'
* command in minishell
* receives an argument that is not a valid numeric value.
* The '%s' placeholder should be replaced with the invalid argument.
*
* Example usage:
*   printf(ERROR_NUM_ARG_REQ, arg);
*
* SPANISH:
*
* @brief Mensaje de error para argumento no numérico en el comando 'exit'.
*
* Este macro define el mensaje de error que se muestra cuando el comando
* 'exit' en minishell
* recibe un argumento que no es un valor numérico válido.
* El marcador '%s' debe ser reemplazado por el argumento inválido.
*
* Ejemplo de uso:
*   printf(ERROR_NUM_ARG_REQ, arg);
*/
# define ERROR_NUM_ARG_REQ "minishell: exit: %s: numeric argument required\n"

/**
* @def HEREDOC_PROMPT
*
* ENGLISH:
*
* @brief Prompt string displayed for heredoc input in minishell.
*
* This macro defines the prompt shown to the user when minishell is
* reading input for a heredoc (here-document). The prompt helps the user
* distinguish heredoc input from regular shell input.
*
* Example usage:
*   printf("%s", HEREDOC_PROMPT);
*
* SPANISH:
*
* @brief Cadena de texto mostrada como prompt para la entrada heredoc en
*	minishell.
*
* Este macro define el prompt que se muestra al usuario cuando minishell está
* leyendo la entrada para un heredoc (here-document). El prompt ayuda al
* usuario a distinguir la entrada heredoc de la entrada normal del shell.
*
* Ejemplo de uso:
*   printf("%s", HEREDOC_PROMPT);
*/
# define HEREDOC_PROMPT "heredoc> "

/**
 * @def ERROR_HEREDOC_PROCESS
 *
 * ENGLISH:
 *
 * @brief Error message for heredoc processing failure in minishell.
 *
 * This macro defines the error message displayed when there is an issue
 * processing a heredoc (here-document) in minishell.
 *
 * Example usage:
 *  fprintf(stderr, ERROR_HEREDOC_PROCESS);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para fallo en el procesamiento de heredoc en
 * 	minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando hay un problema
 * procesando un heredoc (here-document) en minishell.
 *
 * Ejemplo de uso:
 * fprintf(stderr, ERROR_HEREDOC_PROCESS);
 */
# define ERROR_HEREDOC_PROCESS "Error processing heredoc\n"

/**
 * @def ERROR_HEREDOC_DELIMITER
 *
 * ENGLISH:
 *
 * @brief Error message for missing heredoc delimiter in minishell.
 *
 * This macro defines the error message displayed when a heredoc (here-document)
 * in minishell is missing its required delimiter.
 *
 * Example usage:
 * fprintf(stderr, ERROR_HEREDOC_DELIMITER);
 *
 * SPANISH:
 *
 * @brief Mensaje de error para delimitador faltante en heredoc en minishell.
 *
 * Este macro define el mensaje de error que se muestra cuando un heredoc
 * 	(here-document)
 * en minishell no tiene su delimitador requerido.
 *
 * Ejemplo de uso:
 * fprintf(stderr, ERROR_HEREDOC_DELIMITER);
 */
# define ERROR_HEREDOC_DELIMITER "Error: missing delimiter for heredoc\n"

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
}		t_data;

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
}		t_cmd;

/*
** ===================================================================
**                        FUNCTIONS
** ===================================================================
*/

int		ft_minishell(char **envp, int debug);

// Search for a file in the current directory and subdirectories.
char	*ft_search_file(const char *dir, const char *filename);
char	*ft_search_in_subdirs(const char *dir, const char *filename);
char	*ft_search_in_dir(const char *dir, const char *filename);
char	*ft_build_path(const char *dir, const char *entry);

// Parsing

char	**ft_split_input(const char *input, int argc);
t_cmd	*ft_parse_input(char **argv, t_data *data);
void	ft_skip_quotes(const char *cmd, int *i);
char	**ft_handle_env_expansion(char **argv, t_data *data);
char	*ft_remove_quotes(const char *str);
int		ft_check_syntax_errors(char **argv, int argc);
int		ft_handle_quoted_arg(char *arg, int *start, int *end);

// Execution
int		ft_exec_cmd(t_cmd *cmd);
int		ft_is_builtin(t_cmd *current);
char	*ft_get_cmd_path(char *cmd);
void	ft_close_unused_fds(t_cmd *current_cmd, t_cmd *cmd_list);
void	ft_finish_execution(pid_t *pids, t_cmd *cmd_list, t_data *data);
void	ft_setup_child_io(t_cmd *current, t_cmd *cmd_list);
int		ft_exec_builtin(t_cmd *cmd, t_data **data);

// ENV

char	**ft_dupenv(char **envp);
void	ft_free_envp(char **envp);
char	*ft_getenv(const char *name, char **envp);
char	**ft_realloc_envp(char **envp, int new_size);
int		ft_setenv(char *name, char *value, char ***envp);
void	ft_update_pwd_env(char *oldpwd, char *target_dir, char ***envp);
int		ft_update_existing_env(char *name, char *value, char **envp);
char	*ft_create_env_var(char *name, char *value);

// PROMPT

char	*ft_get_directory_path(char **envp);
char	*ft_generate_prompt(char **envp);

// Initialize message
int		ft_msg_start(void);

// Count the number of arguments in a command string.
int		ft_count_args(const char *cmd);

// Memory management utilities
void	ft_free_matrix(char **array);
void	ft_free_matrix_size(char **array, int size);

// INPUT

int		ft_read_input(char **input, t_data *data);
t_cmd	*ft_process_input(char *input, t_data *data, int debug);

// TOKEN UTILS
int		ft_is_redir_token(const char *s);
int		ft_is_pipe_token(const char *s);
int		ft_is_logical_token(const char *s);
int		ft_is_background_token(const char *s);
int		ft_is_operator_token(const char *s);

// EXECUTION

int		ft_execute_pipeline(t_cmd *cmd_list, t_data **data);
int		ft_execute_error_command(t_cmd *cmd_list, t_cmd *head, pid_t *pids);
int		ft_exec_cmd(t_cmd *cmd);

// DEBUG

void	ft_show_debug(char **argv, int argc, char **expanded_argv,
			t_cmd *cmd_list);

// Redirections
int		ft_handle_infile(char *filename);
int		ft_handle_outfile(char *filename, int append);

// Signals
void	sigint_handler(int sig);
void	ft_init_signals(void);

// Builtins
int		ft_echo(t_cmd cmd);
int		ft_cd(char **argv, char ***envp);
int		ft_pwd(t_cmd cmd);
int		ft_export(char **args, char ***envp);
int		ft_unset(char **args, char ***envp);
int		ft_env(t_cmd cmd, char **envp);
int		ft_exit(t_cmd *cmd);

int		ft_handle_builtins(t_cmd *cmd, t_data **data, t_cmd *cmd_list,
			pid_t *pids);

int		ft_is_dot_or_dotdot(const char *name);

// Heredoc
int		ft_handle_heredoc(const char *delimiter);

// Error handling
int		ft_handle_error(int error_code, int exit_code, char *msg, char *msg2);

#endif
