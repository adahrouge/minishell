/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:19:11 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/20 21:39:24 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void create_pipes(t_data *p)
{
	int i;

	p->pipefd = malloc(sizeof(int *) * (p->nb_of_pipes)); //have to free
	if (!p->pipefd)
	{
		perror("malloc failed for p->pipefd");
		exit(EXIT_FAILURE);
	}
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
    p->full_path_pipe = NULL;
    parse_pipes(p);                // sets p->store_pipe_arg, p->num_commands
    create_pipes(p);               // creates p->nb_of_pipes = p->num_commands - 1 pipes

    // Instead of forking and waiting in the same loop,
    // we do two loops: 
    //  (A) Fork all children
    //  (B) Wait for them

    // Let's store child PIDs in an array
    pid_t *pids = malloc(sizeof(pid_t) * p->num_commands);
    if (!pids)
        exit(EXIT_FAILURE);

    // 1) Fork each command
    for (int i = 0; i < p->num_commands; i++)
    {
        pids[i] = fork();
        if (pids[i] < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pids[i] == 0)  // Child
        {
            // Hook up pipes (child side)
            if (i == 0)               // first command
                first_command(p, i);
            else if (i == p->num_commands - 1) // last
                last_command(p, i);
            else
                middle_commands(p, i);

            // Build path + execve
            create_path_pipes(p, head, i);
            execute_command_pipes(p, head, i);
        }
        else // Parent
        {
            // IMPORTANT: Do NOT wait here
            // Instead, just close the parent's copy
            // of pipe ends that won't be used.
            if (i == 0)
            {
                // parent doesn't need pipefd[0][1] after child has it
                if (p->num_commands > 1)
                    close(p->pipefd[0][1]);
            }
            else if (i < p->num_commands - 1)
            {
                // middle command, close pipefd[i][1] in parent
                close(p->pipefd[i][1]);
                // also close pipefd[i-1][0] if still open
            }
            else
            {
                // last command
                close(p->pipefd[i-1][0]);
            }
        }
    }

    // 2) After forking all commands, do a loop to wait for each
    for (int i = 0; i < p->num_commands; i++)
        waitpid(pids[i], NULL, 0);

    free(pids);

    // Then do your cleanup
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
