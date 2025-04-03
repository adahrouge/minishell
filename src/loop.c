/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:44:33 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/03 20:26:10 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_or_command(t_data *p, t_export **head)
{
	char	*path_env;
	int		is_echo;

	is_echo = 0;
	path_env = NULL;
	if (input_contains_pipe(p) && pipe_input_correct(p))
	{
		pipes(p, *head);
		free(p->input);
		return ;
	}
	else if (input_contains_pipe(p))
	{
		free(p->input);
		return ;
	}
		read_command_line(p);
		if (has_slash(p->cmd_args[0]))
		{
			executable(p, *head);
			free_split(p->cmd_args);
			free(p->input);
			return ;
		}
		if (p->cmd_args[0] && ft_strcmp(p->cmd_args[0], "echo") == 0)
		is_echo = 1;
		if (!is_echo)
		{
			expand_all_tokens(p->cmd_args, *head);
			remove_quotes_args(p->cmd_args);
		}
		if (p->cmd_args[0] && is_builtin(p->cmd_args[0]))
			build_in(p, head);
		else
			external_commands(p, *head, path_env);
		free_split(p->cmd_args);
		free(p->input);
}

void	remove_quotes_args(char **args)
{
	int i;

	i = 0;
	while (args[i])
	{
		remove_quotes(args[i]);
		i++;
	}
}

void	remove_quotes(char *arg)
{
	int	src;
	int	dst;

	src = 0;
	dst = 0;
	while (arg[src])
	{
		if (arg[src] != '"' && arg[src] != '\'')
		{
			arg[dst] = arg[src];
			dst++;
		}
		src++;
	}
	arg[dst] = '\0';
}

int	check_loop_result(t_data *p)
{
	if (input_is_null(p))
		return (-1);
	if (input_is_backslash(p))
		return (1);
	if (input_is_space(p))
		return (1);
	if (input_is_redirect(p))
		return (1);
	// if (input_is_slash(p))
	// 	return (1);
	if (input_is_dash(p))
		return (1);
	if (input_is_and(p))
		return (1);
	if (input_is_clear(p))
		return (1);
	return (0);
}

int	read_line(t_data *p)
{
	p->input = readline("minishell > ");
	if (p->input == NULL)
	{
		printf("exit\n");
		return (0);
	}
	return (1);
}

void	loop(t_data *p, t_export **head)
{
	int	special_result;

	arrange_export(*head);
	update_shlvl(head);
	p->exit_code = 0;
	while (1)
	{
		if (!read_line(p))
			break ;
		special_result = check_loop_result(p);
		if (special_result == -1)
			break ;
		else if (special_result == 1)
			continue ;
		add_history(p->input);
		handle_pipe_or_command(p, head);
	}
}
