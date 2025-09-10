/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/08 00:45:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/10 11:15:56 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(const char *str, char c)
{
	int		start;
	int		end;
	int		len;
	char	*trimmed;
	int		i;

	if (!str)
		return (NULL);
	start = 0;
	end = ft_strlen(str) - 1;
	// Encontrar el primer carácter que no sea 'c'
	while (str[start] && str[start] == c)
		start++;
	// Encontrar el último carácter que no sea 'c'
	while (end >= start && str[end] == c)
		end--;
	// Calcular la longitud de la cadena trimmed
	len = end - start + 1;
	if (len <= 0)
		return (ft_strdup(""));
	// Asignar memoria para la nueva cadena
	trimmed = malloc((len + 1) * sizeof(char));
	if (!trimmed)
		return (NULL);
	// Copiar la parte trimmed
	i = 0;
	while (i < len)
	{
		trimmed[i] = str[start + i];
		i++;
	}
	trimmed[i] = '\0';
	return (trimmed);
}
