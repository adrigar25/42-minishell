/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_trim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agarcia <agarcia@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 17:30:00 by agarcia           #+#    #+#             */
/*   Updated: 2025/09/05 17:25:53 by agarcia          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <stdlib.h>
#include <string.h>

char	*ft_trim(const char *str, char c)
{
	char	*result;
	int		start;
	int		end;
	int		len;
	int		i;

	if (!str)
		return (NULL);
	start = 0;
	while (str[start] && str[start] == c)
		start++;
	end = strlen(str) - 1;
	while (end >= start && str[end] == c)
		end--;
	len = end - start + 1;
	if (len <= 0)
		return (ft_strdup(""));
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len)
	{
		result[i] = str[start + i];
		i++;
	}
	result[len] = '\0';
	return (result);
}
