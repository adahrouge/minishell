/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:44:33 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/04 15:37:24 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_or_command(t_data *p, t_export **head)
{
	if (input_contains_pipe(p) && pipe_input_correct(p))
	{
		pipes(p, *head);
		free(p->input);
		return ;
	}
	if (input_contains_pipe(p))
	{
		free(p->input);
		return ;
	}
	read_command_line(p);
	handle_command_execution(p, head);
}

void	handle_command_execution(t_data *p, t_export **head)
{
	char	*path_env;
	int		is_echo;

	is_echo = 0;
	path_env = NULL;
	if (has_slash(p->cmd_args[0]))
	{
		executable_main(p, head);
		return ;
	}
	if (p->cmd_args[0] && ft_strcmp(p->cmd_args[0], "echo") == 0)
		is_echo = 1;
	if (!is_echo)
		trim_arg(p, head);
	if (p->cmd_args[0] && is_builtin(p->cmd_args[0]))
		build_in(p, head);
	else
		external_commands(p, *head, path_env);
	free_split(p->cmd_args);
	free(p->input);
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
