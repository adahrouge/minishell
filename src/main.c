/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:03:33 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/20 16:26:12 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	read_command_line(t_data *p)
{
	p->args = count_words(p->input, ' ');
	p->cmd_args = split_cmd_quoted(p->input);
}

void	free_allocated(t_data *p)
{
	int	i;

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

int	main(int argc, char **argv, char **environ)
{
	t_data		*p;
	t_export	*head;
	char		**my_environ;

	my_environ = copy_environ(environ);
	setup_signal_handlers();
	if (argc != 1 && ft_strcmp(argv[0], "./minishell") != 0)
		return (0);
	p = malloc(sizeof(t_data));
	if (!p)
		return (0);
	head = populate_list(my_environ);
	if (!head)
	{
		free(p);
		free_environ(my_environ);
		return (0);
	}
	arrange_export(head);
	loop(p, &head);
	free(p);
	free_environ(my_environ);
	free_list(head);
	return (0);
}

int	free_environ(char **my_environ)
{
	int	i;

	i = 0;
	while (my_environ[i] != NULL)
	{
		free(my_environ[i]);
		i++;
	}
	free(my_environ);
	return (1);
}
