/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:46:27 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/28 12:25:21 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_echo(char *arg)
{
	int i;
	int consumed;
	
	i = 0;
	consumed = 0;
	while (arg[i] != '\0')
	{
		while (arg[i] != '\'' && arg[i] != '"' && arg[i] != '$' && arg[i] != '\0')
		{
			write(1, &arg[i], 1);
			i++;
		}
		if (arg[i] == '\'' || arg[i] == '"')
		{
			consumed = handle_quotations(&arg[i]);
			i  = i + consumed;
			continue;
		}
		if (arg[i] == '$')
		{
			i++;
			consumed = handle_dollar(&arg[i]);
		    i = i + consumed;
			continue;
		}
		if (arg[i] != '\0')
			i++;
	}
}
int handle_dollar(char *arg)
{
	int i;
	int consumed;
	consumed = 0;
	i = 0;
	   if (arg[i] > '0' && arg[i] <= '9')
        {
			while (arg[i] != ' ')
			{
				write(1, &arg[i], 1);
				i++;
			}
			consumed = i;
		}
		else
		{
		consumed = expand_variable(&arg[i]);
		if (consumed == -1)
			return i;
		i = i + consumed;
		consumed = i;
		}
		return consumed;
}
int echo(t_data *p)
{
	int flag_n;
    char *arg;

	flag_n = 0;
	if (ft_strncmp(p->input, "echo -n", 7) == 0)
	{
		arg = ft_strdup(p->input + 8);
		flag_n = 1;
	}
	else if (ft_strncmp(p->input, "echo", 4) == 0)
		arg = ft_strdup(p->input + 5);
	else
		return 0;
	if (!arg)
		return 0;
	if (!check_echo(arg))
		return 0;
	parse_echo(arg);
	if (!flag_n)
		write(1, "\n", 1);
	free(arg);
	return 1;
}
