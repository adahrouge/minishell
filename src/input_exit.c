/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_exit.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 15:20:36 by adahroug          #+#    #+#             */
/*   Updated: 2025/03/08 17:24:21 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


int	input_is_exit(t_data *p)
{
	if (ft_strcmp(p->input, "exit") == 0)
		return (1);
	return (0);
}
int is_numeric(char *str)
{
	int i;

	i = 0;
	if (str[0] == '\0')
		return (0);
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
void exit_print(t_data *p, char *arg)
{
	write(2, "bash: exit: ", 12);
	write(2, arg, ft_strlen(arg));
	write(2, ": numeric argument needed\n", 27);
	p->exit_code = 2;
	cleanup_and_exit(p, 2);
}
int exit_print_too_many(t_data *p)
{
	write(2, "bash: exit: too many arguments\n", 31);
	p->exit_code = 1;
	return 1;
}
int exit_main(t_data *p)
{
	int argc;
	int code;

	write(1, "exit\n", 5);
	argc = count_tokens_2d(p->cmd_args);
	if (argc == 1)
		cleanup_and_exit(p, 0);
	if (argc > 2)
	{
		if (!is_numeric(p->cmd_args[1]))
			exit_print(p, p->cmd_args[1]);
		exit_print_too_many(p);
		return (1);
	}
	if (!is_numeric(p->cmd_args[1]))
		exit_print(p, p->cmd_args[1]);
	code = atoi(p->cmd_args[1]) % 256;
	cleanup_and_exit(p, code);
	return 1;
}

int count_tokens_2d(char **arg)
{
	int i;

	i = 0;
	while (arg[i] != NULL)
		i++;
	return i;
}
void cleanup_and_exit(t_data *p, int code)
{
	if (p->input)
		free(p->input);
	exit(code);
}
