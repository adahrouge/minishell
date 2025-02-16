/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 14:44:33 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/16 17:07:35 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe_or_command(t_data *p, t_export **head)
{
	char	*path_env;

	path_env = NULL;
	if (input_contains_pipe(p) && pipe_input_correct(p))
	{
		pipes(p, *head);
		free(p->input);
	}
	else if (input_contains_pipe(p))
	{
		free(p->input);
	}
	else
	{
		read_command_line(p);
		if (p->cmd_args[0] && is_builtin(p->cmd_args[0]))
			build_in(p, head);
		else
			external_commands(p, *head, path_env);
		free_split(p->cmd_args);
		free(p->input);
	}
}

int	check_loop_result(t_data *p)
{
	if (input_is_null(p))
		return (-1);
	if (input_is_backslash(p))
		return (1);
	if (input_is_exit(p))
		return (-1);
	if (input_is_space(p))
		return (1);
	if (input_is_redirect(p))
		return (1);
	if (input_is_slash(p))
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
	if (p->input[0] == '\0')
	{
		free(p->input);
		return (1);
	}
	return (1);
}

void	loop(t_data *p, t_export **head)
{
	int	special_result;

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
// void loop(t_data *p, t_export **head)
// {
//     while (1)
//     {
//         p->input = readline("minishell > ");
// 		if (p->input == NULL)
// 		{
// 			printf("exit\n");
// 			break;	
// 		}
// 		if (p->input[0] == '\0')
// 		{
// 			free(p->input);
// 			continue;
// 		}
// 		if (input_is_null(p))
// 			break;
//         if (input_is_backslash(p))
//             continue;
//         if (input_is_exit(p))
//             break;
// 		if (input_is_space(p))
// 			continue;
// 		if (input_is_redirect(p))
// 			continue;
// 		if (input_is_slash(p))
// 			continue;
// 		if (input_is_dash(p))
// 			continue;
// 		if (input_is_and(p))
// 			continue;
// 		if (input_is_clear(p))
// 			continue;
//         add_history(p->input);
// 		if (input_contains_pipe(p) && pipeInputCorrect(p))
// 		{
// 			pipes(p, *head);
// 			continue;
// 		}
// 		else if (input_contains_pipe(p))
// 		{
// 			free(p->input);
// 			continue;
// 		}
// 		else
// 		{
//         read_command_line(p);
// 		if (p->cmd_args[0] && is_builtin(p->cmd_args[0]))
//         	build_in(p, head);
// 			else
// 			 external_commands(p, *head);
// 		free_split(p->cmd_args);
//         free(p->input);
// 		}
// 	}
// }
