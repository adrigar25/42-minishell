/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_env_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/22 10:53:22 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Appends the source string to the destination string, updating the destination.
 *
 * SPANISH: Añade la cadena fuente a la cadena destino, actualizando la destino.
 *
 * @param dst   Pointer to the destination string. /
 *              Puntero a la cadena destino.
 *
 * @param src   The source string to append. /
 *              La cadena fuente a añadir.
 *
 * @returns 1 if the append was successful, 0 otherwise. /
 *          1 si la concatenación fue exitosa, 0 en caso contrario.
 */
static int	ft_append(char **dst, char *src)
{
	char	*old;

	old = *dst;
	*dst = ft_strjoin(old, src);
	free(old);
	return (*dst != NULL);
}

/**
 * ENGLISH: Expands the special variable $? with the last exit status.
 *
 * SPANISH: Expande la variable especial $? con el último estado de salida.
 *
 * @param dst   Pointer to the destination string. /
 *              Puntero a la cadena destino.
 *
 * @param j     Pointer to the current index in the argument string. /
 *              Puntero al índice actual en la cadena de argumentos.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns 1 if the expansion was successful, 0 otherwise. /
 *          1 si la expansión fue exitosa, 0 en caso contrario.
 */
static int	ft_expand_exit_status(char **dst, int *j, t_data *data)
{
	char	*temp;

	temp = ft_itoa(data->last_exit_status);
	if (!temp)
		return (0);
	if (!ft_append(dst, temp))
	{
		free(temp);
		return (0);
	}
	free(temp);
	(*j)++;
	return (1);
}

/**
 * ENGLISH: Expands an environment variable in the argument string.
 *
 * SPANISH: Expande una variable de entorno en la cadena de argumentos.
 *
 * @param dst   Pointer to the destination string. /
 *              Puntero a la cadena destino.
 *
 * @param arg   The argument string containing the variable to expand. /
 *              La cadena de argumentos que contiene la variable a expandir.
 *
 * @param j     Pointer to the current index in the argument string. /
 *              Puntero al índice actual en la cadena de argumentos.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns 1 if the expansion was successful, 0 otherwise. /
 *          1 si la expansión fue exitosa, 0 en caso contrario.
 */
static int	ft_expand_env(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	(*j)++;
	if (arg[*j] == '?')
		return (ft_expand_exit_status(dst, j, data));
	start = *j;
	while (arg[*j] && (ft_isalnum(arg[*j]) || arg[*j] == '_'))
		(*j)++;
	if (*j == start)
		return (ft_append(dst, "$"));
	env_name = ft_substr(arg, start, *j - start);
	if (!env_name)
		return (0);
	env_value = ft_getenv(env_name, data->envp);
	free(env_name);
	if (env_value && !ft_append(dst, env_value))
		return (0);
	return (1);
}

/**
 * ENGLISH: Processes a single argument, handling environment variable expansion.
 *
 * SPANISH: Procesa un solo argumento, manejando la expansión de variables de entorno.
 *
 * @param dst   Pointer to the destination string. /
 *              Puntero a la cadena destino.
 *
 * @param arg   The argument string to process. /
 *              La cadena de argumentos a procesar.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns 1 if the processing was successful, 0 otherwise. /
 *          1 si el procesamiento fue exitoso, 0 en caso contrario.
 */
static int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int		j;
	int		start;
	char	*temp;

	j = 0;
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return ((*dst = ft_strdup(arg)) != NULL);
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if (j > start)
		{
			temp = ft_substr(arg, start, j - start);
			if (!ft_append(dst, temp))
			{
				free(temp);
				return (0);
			}
			free(temp);
		}
		if (arg[j] == '$' && !ft_expand_env(dst, arg, &j, data))
			return (0);
	}
	return (1);
}

/**
 * ENGLISH: Handles environment variable expansion for an array of arguments.
 *
 * SPANISH: Maneja la expansión de variables de entorno para un array de argumentos.
 *
 * @param argv  The array of argument strings. /
 *              El array de cadenas de argumentos.
 *
 * @param data  Pointer to the shell data structure. /
 *              Puntero a la estructura de datos del shell.
 *
 * @returns A new array of argument strings with variables expanded, or the original array on error. /
 *          Un nuevo array de cadenas de argumentos con las variables expandidas, o el array original en caso de error.
 */
char	**ft_handle_env_expansion(char **argv, t_data *data)
{
	int		i;
	char	**new_argv;

	if (!argv || !data || !data->envp)
		return (argv);
	new_argv = malloc(sizeof(char *) * (data->argc + 1));
	if (!new_argv)
		return (argv);
	i = 0;
	while (argv[i])
	{
		new_argv[i] = ft_strdup("");
		if (!new_argv[i] || !ft_process_arg(&new_argv[i], argv[i], data))
		{
			ft_free_matrix(new_argv);
			return (argv);
		}
		i++;
	}
	new_argv[i] = NULL;
	return (new_argv);
}
