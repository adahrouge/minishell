/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:46:27 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/11 13:18:49 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void parse_echo(char *arg, t_export *head)
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
			consumed = handle_quotations(&arg[i], head);
			i  = i + consumed;
			continue;
		}
		if (arg[i] == '$')
		{
			i++;
			consumed = handle_dollar(&arg[i], head);
		    i = i + consumed;
			continue;
		}
		if (arg[i] != '\0')
			i++;
	}
}
int handle_dollar(char *arg, t_export *head)
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
		consumed = expand_variable(&arg[i], head);
		if (consumed == -1)
			return i;
		i = i + consumed;
		consumed = i;
		}
		return consumed;
}
int echo(t_data *p, t_export *head)
{
	int flag_n;
    char *arg;
	if (ft_strcmp(p->input, "echo $?") == 0)
	{
		printf("%d\n", p->exit_code);
		return 0;
	}
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
	parse_echo(arg, head);
	if (!flag_n)
		write(1, "\n", 1);
	free(arg);
	return 0;
}
