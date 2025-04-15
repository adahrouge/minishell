/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/28 12:23:59 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 17:29:36 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_quotations(char *arg, int *i, t_export *head)
{
	if (arg[*i] == '\'')
	{
		return (handle_single_quotes(arg));
	}
	else if (arg[*i] == '"')
	{
		return (handle_double_quotes(arg, head));
	}
	return (1);
}

int	handle_single_quotes(char *arg)
{
	int	i;

	i = 1;
	while (arg[i] != '\'')
	{
		write(1, &arg[i], 1);
		i++;
	}
	if (arg[i] == '\'')
		i++;
	return (i);
}

int	handle_double_quotes(char *arg, t_export *head)
{
	int	i;
	int	consumed;

	i = 1;
	while (arg[i] != '"' && arg[i] != '\0')
	{
		if (arg[i] == '$')
		{
			i++;
			consumed = expand_variable(&arg[i], head);
			i = i + consumed;
			continue ;
		}
		else
			write(1, &arg[i], 1);
		i++;
	}
	if (arg[i] == '"')
		i++;
	return (i);
}

int	check_echo(char *arg)
{
	int	i;

	i = 0;
	if (arg[0] == '\0')
	{
		write(1, "\n", 1);
		return (0);
	}
	while (arg[i] == ' ')
		i++;
	if (arg[i] == '\0')
	{
		write(1, "\n", 1);
		return (0);
	}
	return (1);
}

int	expand_variable(char *arg, t_export *head)
{
	int		i;
	char	var_name[256];
	int		var_len;
	char	*value;

	var_len = 0;
	i = 0;
	while ((arg[i] >= 'A' && arg[i] <= 'Z')
		|| (arg[i] >= 'a' && arg[i] <= 'z')
		|| (arg[i] >= '0' && arg[i] <= '9')
		|| arg[i] == '_')
		var_name[var_len++] = arg[i++];
	var_name[var_len] = '\0';
	value = my_getenv(var_name, head);
	if (value)
		write(1, value, ft_strlen(value));
	return (i);
}
