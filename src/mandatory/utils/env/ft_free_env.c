/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/22 13:58:37 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/22 16:32:50 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

/**
 * ENGLISH: Frees a NULL-terminated array of environment variable strings.
 *
 * SPANISH: Libera un arreglo de cadenas de variables de entorno terminado
 *  		en NULL.
 *
 * @param envp   The NULL-terminated array of environment variable strings
 *               to free. /
 *               El arreglo de cadenas de variables de entorno terminado en
 *               NULL a liberar.
 */
void	ft_free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}
