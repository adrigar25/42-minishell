/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_env_expansion.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>             +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 21:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/24 12:00:00 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**

	* Copies a literal substring from arg[start] to
	arg[end-1] and appends it to dst.
 *
 * @param dst   Pointer to the destination string.
 * @param arg   The argument string.
 * @param start Start index (inclusive).
 * @param end   End index (exclusive).
 * @returns 1 on success, 0 on failure.
 */
int	ft_copy_literal(char **dst, char *arg, int start, int end)
{
	char	*literal;
	char	*temp;

	if (end <= start)
		return (1);
	literal = ft_substr(arg, start, end - start);
	if (!literal)
		return (0);
	temp = *dst;
	*dst = ft_strjoin(temp, literal);
	free(temp);
	free(literal);
	if (!*dst)
		return (0);
	return (1);
}

/**
 * ENGLISH: Expands the special variable $? with the last exit status.
 *
 * SPANISH: Expande la variable especial $? con el último estado de salida.
 *
 * @param dst   Pointer to the destination string.
 * @param j     Pointer to the current index.
 * @param data  Pointer to the shell data.
 * @returns 1 on success, 0 on failure.
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
	(*j) += 2;
	return (1);
}

/**
 * ENGLISH: Expands an environment variable.
 *
 * SPANISH: Expande una variable de entorno.
 *
 * @param dst   Pointer to the destination string.
 * @param arg   The argument string.
 * @param j     Pointer to the current index.
 * @param data  Pointer to the shell data.
 * @returns 1 on success, 0 on failure.
 */
static int	ft_expand_env_var(char **dst, char *arg, int *j, t_data *data)
{
	int		start;
	char	*env_name;
	char	*env_value;

	(*j)++;
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
	if (env_value)
		if (!ft_append(dst, (char *)env_value))
			return (0);
	return (1);
}

/**
 * ENGLISH: Processes a single argument for environment expansion.
 *
 * SPANISH: Procesa un solo argumento para expansión de entorno.
 *
 * @param dst   Pointer to the destination string.
 * @param arg   The argument string.
 * @param data  Pointer to the shell data.
 * @returns 1 on success, 0 on failure.
 */
static int	ft_process_arg(char **dst, char *arg, t_data *data)
{
	int	j;
	int	start;

	j = 0;
	if (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\'')
		return ((*dst = ft_strdup(arg)) != NULL);
	while (arg[j])
	{
		start = j;
		while (arg[j] && arg[j] != '$')
			j++;
		if (j > start && !ft_copy_literal(dst, arg, start, j))
			return (0);
		if (arg[j] == '$')
		{
			if (arg[j + 1] == '?')
			{
				if (!ft_expand_exit_status(dst, &j, data))
					return (0);
			}
			else if (!ft_expand_env_var(dst, arg, &j, data))
				return (0);
		}
	}
	return (1);
}

/**
 * ENGLISH: Handles environment variable expansion for arguments.
 *
 * SPANISH: Maneja la expansión de variables de entorno para argumentos.
 *
 * @param argv  The array of arguments.
 * @param data  Pointer to the shell data.
 * @returns New array with expanded variables or original on error.
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
