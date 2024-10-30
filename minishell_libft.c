/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_libft.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:24:07 by adahroug          #+#    #+#             */
/*   Updated: 2024/10/30 09:27:29 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_strcmp(char *s1, char *s2)
{
	int i;
	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			return s1 - s2;
		i++;
	}
	return s1[i] - s2[i];
}
void	ft_strcpy(char *dest, const char *src)
{
	int	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}
int ft_strlen(const char *str)
{
	int i;
	i = 0;
	while (str[i] != '\0')
	{
		i++;
	}
	return i;
}
