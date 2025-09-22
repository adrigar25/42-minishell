/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_show_debug.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/12 17:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 16:31:30 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Counts the number of arguments in a NULL-terminated array of strings.
 *
 * SPANISH: Cuenta el número de argumentos en un arreglo de cadenas
 * 			terminado en NULL.
 *
 * @param argv   The NULL-terminated array of strings.
 *               El arreglo de cadenas terminado en NULL.
 *
 * @returns The number of arguments in the array.
 *          El número de argumentos en el arreglo.
 */
static int	ft_count_args_from_array(char **argv)
{
	int	count;

	count = 0;
	if (!argv)
		return (0);
	while (argv[count])
		count++;
	return (count);
}

/**
 * ENGLISH: Displays the contents of an argument array with a title and prefix.
 *
 * SPANISH: Muestra el contenido de un arreglo de argumentos
 * 			con un título y prefijo.
 *
 * @param title   The title to display before the arguments.
 *                El título que se muestra antes de los argumentos.
 *
 * @param prefix  The prefix for each argument line.
 *                El prefijo para cada línea de argumento.
 *
 * @param argv    The array of argument strings.
 *                El arreglo de cadenas de argumentos.
 *
 * @param argc    The number of arguments in the array.
 *                El número de argumentos en el arreglo.
 */
static void	ft_show_argv(char *title, char *prefix, char **argv, int argc)
{
	int	i;

	printf("%s:\n", title);
	i = 0;
	while (i < argc)
	{
		printf("  %s[%d]: %s\n", prefix, i, argv[i]);
		i++;
	}
}

/**
 * ENGLISH: Displays the details of a linked list of command structures.
 *
 * SPANISH: Muestra los detalles de una lista enlazada de estructuras de comando.
 *
 * @param cmd_list   The head of the linked list of command structures.
 *                   La cabeza de la lista enlazada de estructuras de comando.
 */
static void	ft_show_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*curr;
	int		arg_count;

	curr = cmd_list;
	while (curr)
	{
		printf("cmd: \n");
		arg_count = 0;
		if (curr->argv)
			arg_count = ft_count_args_from_array(curr->argv);
		ft_show_argv("  argv", "argv", curr->argv, arg_count);
		printf("  fd_in: %d, fd_out: %d\n", curr->infd, curr->outfd);
		printf("  has_error: %d\n", curr->has_error);
		curr = curr->next;
		if (curr)
			printf("  |\n  v\n");
	}
}

/**
 * ENGLISH: Displays debug information including received arguments,
 *          expanded arguments, and parsed command structures.
 *
 * SPANISH: Muestra información de depuración incluyendo argumentos recibidos,
 *          argumentos expandidos y estructuras de comando parseadas.
 *
 * @param argv           The original argument array received by the shell. /
 *                       El arreglo original de argumentos recibido por el shell.
 *
 * @param argc           The number of arguments in the original array. /
 *                       El número de argumentos en el arreglo original.
 *
 * @param expanded_argv  The argument array after expansion
 *                       (e.g., variable expansion). /
 *                       El arreglo de argumentos después de la expansión
 *                       (p.ej., expansión de variables).
 *
 * @param cmd_list       The linked list of parsed command structures. /
 *                       La lista enlazada de estructuras de comando parseadas.
 */
void	ft_show_debug(char **argv, int argc, char **expanded_argv,
		t_cmd *cmd_list)
{
	int	i;

	printf("\033[38;5;208m========== DEBUG INFO ==========\033[0m\n");
	if (argv)
	{
		printf("\033[34mArgumentos recibidos:\033[0m\n");
		i = 0;
		while (i < argc)
		{
			printf("  argv[%d]: %s\n", i, argv[i]);
			i++;
		}
	}
	if (expanded_argv)
	{
		printf("\n\033[34mArgumentos después de expandir:\033[0m\n");
		ft_show_argv("Expanded argv", "expanded_argv", expanded_argv, argc);
	}
	if (cmd_list)
	{
		printf("\n\033[34mComandos parseados:\033[0m\n");
		ft_show_cmd_list(cmd_list);
	}
	printf("\033[38;5;208m================================\033[0m\n\n");
}
