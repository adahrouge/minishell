/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:46:27 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/06 17:57:10 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void echo_print_arg(char *arg, t_export *head)
{
    int i;
    int consumed;

	i = 0;
    while (arg[i] != '\0')
    {
        while (arg[i] != '\0' && arg[i] != '\'' && arg[i] != '"' && arg[i] != '$')
        {
            write(1, &arg[i], 1);
            i++;
        }
        if (arg[i] == '\'' || arg[i] == '"')
        {
            consumed = handle_quotations(&arg[i], head);
            i += consumed;
        }
        else if (arg[i] == '$')
        {
            i++;
            consumed = handle_dollar(&arg[i], head);
            i += consumed;
        }
    }
}
int echo(t_data *p, t_export *head)
{
    int i;
    int no_newline;

	i = 1;
	no_newline = 0;
    while (p->cmd_args[i] && ft_strcmp(p->cmd_args[i], "-n") == 0)
    {
        no_newline = 1;
        i++;
    }
    if (!p->cmd_args[i])
    {
        if (!no_newline)
            write(1, "\n", 1);
        return (0);
    }
    while (p->cmd_args[i])
    {
        echo_print_arg(p->cmd_args[i], head);
        if (p->cmd_args[i + 1])
            write(1, " ", 1);
        i++;
    }
    if (!no_newline)
        write(1, "\n", 1);
    return (0);
}
