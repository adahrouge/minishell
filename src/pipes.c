/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:19:11 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/21 19:35:51 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void create_pipes(t_data *p)
{
	int i;

	p->pipefd = malloc(sizeof(int *) * (p->nb_of_pipes)); //have to free
	if (!p->pipefd)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < p->nb_of_pipes)
	{
		p->pipefd[i] = malloc(2 * sizeof(int)); // have to free
		if (!p->pipefd[i])
		{
			perror("malloc failed for pipefd[i]");
			free_pipe(p, i);
			return ;
		}
		if ((pipe(p->pipefd[i]) == -1))
		{
			perror("piping didnt work");
			free_pipe(p, i);
			return ;
		}
		i++;
	}
	return ;
}
void pipes(t_data *p, t_export *head)
{
    pipe_prepare(p);
    pipe_fork_loop(p, head);
    pipe_wait_loop(p);
    pipe_cleanup(p);
}
void pipe_prepare(t_data *p)
{
    parse_pipes(p);
    create_pipes(p);
    p->pids = malloc(sizeof(pid_t) * p->num_commands);
    if (!p->pids)
        exit(EXIT_FAILURE);
}
void handle_child(t_data *p, t_export *head, int i)
{
    if (i == 0 && p->num_commands > 1)
        first_command(p, i);
    else if (i == p->num_commands - 1)
        last_command(p, i);
    else
        middle_commands(p, i);

    create_path_pipes(p, head, i);
    execute_command_pipes(p, head, i);
}
void handle_parent(t_data *p, int i)
{
    if (i == 0 && p->num_commands > 1)
        close(p->pipefd[0][1]);
    else if (i < p->num_commands - 1)
        close(p->pipefd[i][1]);
    else if (i > 0)
        close(p->pipefd[i - 1][0]);
}
void pipe_fork_loop(t_data *p, t_export *head)
{
    int i;

    i = 0;
    while (i < p->num_commands)
    {
        p->pids[i] = fork();
        if (p->pids[i] < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (p->pids[i] == 0)
            handle_child(p, head, i);
        else
        {
            handle_parent(p, i);
            p->full_path_pipe = NULL;
        }
        i++;
    }
}
void pipe_wait_loop(t_data *p)
{
    int j;

    j = 0;
    while (j < p->num_commands)
    {
        waitpid(p->pids[j], NULL, 0);
        j++;
    }
}
void pipe_cleanup(t_data *p)
{
    free(p->pids);
    free_pipe(p, p->nb_of_pipes);
    free_2d_array(p->store_pipe_arg);

    if (p->full_path_pipe)
    {
        free(p->full_path_pipe);
        p->full_path_pipe = NULL;
    }
    free(p->input);
}



void parse_pipes(t_data *p)
{
	parse_pipe_arg(p);
	create_pipe_arg(p);
	trim_pipe_args(p);
	p->num_commands = p->nb_of_pipes + 1;
}
