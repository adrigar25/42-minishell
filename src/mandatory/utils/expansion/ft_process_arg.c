/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_process_arg.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 10:00:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/11/24 17:46:05 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Handles expansion when a dollar sign is followed by a quote.
 * 		It extracts the content within the quotes and appends it to the
 * 		destination string.
 *
 * SPANISH: Maneja la expansión cuando un signo de dólar es seguido por una
 * 		comilla.
 * 		Extrae el contenido dentro de las comillas y lo añade a la cadena de
 * 		destino.
 *
 * @param dst Pointer to the destination string. /
 * 			Puntero a la cadena de destino.
 * @param arg The argument string containing the dollar sign and quotes. /
 * 			La cadena de argumento que contiene el signo de dólar y las
 * 			comillas.
 * @param j   Pointer to the current index in the argument string. /
 * 			Puntero al índice actual en la cadena de argumento.
 *
 * @returns SUCCESS on successful expansion, or ERROR on memory allocation
 *          failure. /
 *          SUCCESS en caso de expansión exitosa, o ERROR en caso de fallo de
 *          asignación de memoria.
 */
static int	ft_handle_quoted_after_dollar(char **dst, char *arg, int *j)
{
	int		start;
	int		end;
	char	*content;
	int		ret;

	if (!arg[*j + 1] || !arg[*j + 2])
	{
		(*j)++;
		return (ft_append(dst, "$"));
	}
	start = *j + 2;
	end = start;
	while (arg[end] && arg[end] != arg[*j + 1])
		end++;
	if (arg[end] != arg[*j + 1])
		return ((*j)++, ft_append(dst, "$"));
	content = ft_substr(arg, start, end - start);
	if (!content)
		return (ERROR);
	ret = ft_append(dst, content);
	free(content);
	*j = end + 1;
	return (ret);
}

/**
 * ENGLISH: Expands a variable in the argument string starting at index j.
 * 		It handles special cases for quotes, exit status, and process ID.
 * 		Otherwise, it expands environment variables.
 *
 * SPANISH: Expande una variable en la cadena de argumento comenzando en el
 * 		índice j.
 * 		Maneja casos especiales para comillas, estado de salida e ID de
 * 		proceso.
 * 		De lo contrario, expande variables de entorno.
 *
 * @param dst Pointer to the destination string. /
 * 			Puntero a la cadena de destino.
 * @param arg The argument string containing the variable to expand. /
 * 			La cadena de argumento que contiene la variable a expandir.
 * @param j   Pointer to the current index in the argument string. /
 * 			Puntero al índice actual en la cadena de argumento.
 * @param data Pointer to the shell data structure containing environment
 * 			information. /
 * 			Puntero a la estructura de datos del shell que contiene
 * 			información del entorno.
 *
 * @returns SUCCESS on successful expansion, or ERROR on memory allocation
 *          failure. /
 *          SUCCESS en caso de expansión exitosa, o ERROR en caso de fallo de
 *          asignación de memoria.
 */
static int	ft_expand_var(char **dst, char *arg, int *j, t_data *data)
{
	if (arg[*j + 1] == '"' && ft_strchr(arg + *j + 2, '\''))
	{
		(*j)++;
		return (SUCCESS);
	}
	if (arg[*j + 1] == '"')
		return (ft_handle_quoted_after_dollar(dst, arg, j));
	if (arg[*j + 1] == '?')
		return (ft_expand_exit_status(dst, j, data));
	if (arg[*j + 1] == '$')
		return (ft_expand_pid(dst, j, data));
	return (ft_expand_env_var(dst, arg, j, data));
}

/**
 * ENGLISH: Processes an argument string, handling variable expansions and
 * 		literal copying. It returns a new string with all expansions applied.
 *
 * SPANISH: Procesa una cadena de argumento, manejando expansiones de
 * 		variables y copiado literal.
 * 		Devuelve una nueva cadena con todas las expansiones aplicadas.
 *
 * @param arg  The argument string to process. /
 * 			La cadena de argumento a procesar.
 * @param data Pointer to the shell data structure containing environment
 * 			information. /
 * 			Puntero a la estructura de datos del shell que contiene
 * 			información del entorno.
 *
 * @returns A new string with all expansions applied, or NULL on memory
 *          allocation failure. /
 *          Una nueva cadena con todas las expansiones aplicadas, o NULL en
 *          caso de fallo de asignación de memoria.
 */
char	*ft_process_arg(char *arg, t_data *data)
{
	int		j;
	char	*dst;
	int		start;

	if (!arg)
		return (NULL);
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return (ft_substr(arg, 0, ft_strlen(arg)));
	dst = ft_strdup("");
	if (!dst)
		return (NULL);
	j = 0;
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if ((ft_copy_literal(&dst, arg, start, j) == ERROR) || (arg[j] == '$'
				&& (ft_expand_var(&dst, arg, &j, data) == ERROR)))
		{
			free(dst);
			return (NULL);
		}
	}
	return (dst);
}
