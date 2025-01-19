/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:03:33 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/19 16:16:18 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void read_command_line(t_data *p)
{
	p->args = count_words(p->input, ' ');
	p->cmd_args = ft_split(p->input, ' ');
}

void free_allocated(t_data *p)
{
	int i;
	i = 0;
	while (i < p->args)
	{
		free(p->cmd_args[i]);
		i++;
	}
	free(p->cmd_args);
	free(p->input);
	free(p);
}

void loop(t_data *p, t_export **head)
{
    while (1)
    {
        p->input = readline("minishell > ");
		if (p->input == NULL)
		{
			printf("exit\n");
			break;	
			}
		if (p->input[0] == '\0')
		{
			free(p->input);
			continue;
		}
		if (input_is_null(p))
			break;
        if (input_is_backslash(p))
            continue;
        if (input_is_exit(p))
            break;
		if (input_is_space(p))
			continue;
		if (input_is_redirect(p))
			continue;
		if (input_is_slash(p))
			continue;
		if (input_is_dash(p))
			continue;
		if (input_is_and(p))
			continue;
		if (input_is_clear(p))
			continue;
		if (input_contains_pipe(p))
		{
			pipes(p, *head);
			continue;
		}
        add_history(p->input);
        read_command_line(p);
        build_in(p, head);
		free_split(p->cmd_args);
        free(p->input);
	}
}
int main(int argc, char **argv, char **environ)
{
	t_data *p;
	t_export *head;
	char **my_environ;

	my_environ = copy_environ(environ);
	setup_signal_handlers();
	if (argc != 1 && ft_strcmp(argv[0], "./minishell") != 0)
		return 0;
	p = malloc(sizeof(t_data));
	if (!p)
		return 0;
	head = populate_list(my_environ);
	if (!head)
	{
		free(p);
		free_environ(my_environ);
		return 0;
	}
	arrange_export(head);
	loop(p, &head);
	free(p);
	free_environ(my_environ);
	free_list(head);
	return (0);
}
int free_environ(char **my_environ)
{
	int i;
	i = 0;
	while (my_environ[i] != NULL)
	{
		free(my_environ[i]);
		i++;
	}
	free(my_environ);
	return 1;
}
