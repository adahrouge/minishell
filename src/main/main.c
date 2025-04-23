/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:03:33 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/23 15:52:36 by adahroug         ###   ########.fr       */
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

void	free_all(t_data *p, t_export *head, char **my_environ)
{
	free(p);
	free_environ(my_environ);
	free_list(head);
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

int	main(int argc, char **argv, char **environ)
{
	t_data		*p;
	t_export	*head;
	char		**my_environ;

	if (argc != 1 || ft_strcmp(argv[0], "./minishell") != 0)
	{
		printf("arguments aren't valid\n");
		return (0);
	}
	my_environ = copy_environ(environ);
	p = malloc(sizeof(t_data));
	if (!p)
		return (0);
	setup_signal_handlers();
	head = populate_list(my_environ);
	if (!head)
	{
		free(p);
		free_environ(my_environ);
		return (0);
	}
	loop(p, &head);
	free_all(p, head, my_environ);
	return (0);
}
