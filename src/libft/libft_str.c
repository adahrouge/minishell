/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_str.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:58:34 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/04 15:00:29 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*sub;

	if (start >= (unsigned int) ft_strlen(s))
	{
		sub = (char *)malloc(1);
		if (!sub)
			return (NULL);
		sub[0] = '\0';
		return (sub);
	}
	if (len > (unsigned int) ft_strlen(s) - start)
		len = (unsigned int) ft_strlen(s) - start;
	sub = (char *)malloc(len + 1);
	if (!s || !sub)
		return (NULL);
	i = 0;
	while (i < len && s[start] != '\0')
	{
		sub[i] = s[start];
		i++;
		start++;
	}
	sub[i] = '\0';
	return (sub);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*ptr;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	ptr = malloc((len1 + len2 + 1) * sizeof(char));
	if (ptr == NULL)
	{
		return (NULL);
	}
	ft_strcpy(ptr, s1);
	ft_strcpy(ptr + len1, s2);
	ptr[len1 + len2] = '\0';
	return (ptr);
}

static int	ft_isnegative(long *nb)
{
	int	sign;

	sign = 0;
	if (*nb < 0)
	{
		sign = 1;
		*nb = -(*nb);
	}
	return (sign);
}

static int	ft_count_digits(long nb)
{
	int	len;

	len = 1;
	while (nb >= 10)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long	nb;
	int		sign;
	int		len;
	char	*res;

	nb = (long)n;
	sign = ft_isnegative(&nb);
	len = ft_count_digits(nb);
	res = malloc(len + sign + 1);
	if (!res)
		return (NULL);
	res[len + sign] = '\0';
	while (len > 0)
	{
		res[len + sign - 1] = (nb % 10) + '0';
		nb /= 10;
		len--;
	}
	if (sign)
		res[0] = '-';
	return (res);
}
