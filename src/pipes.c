/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:19:11 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/19 16:12:02 by adahroug         ###   ########.fr       */
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
    parse_pipe_args(p);  // fill p->store_pipe_arg
    create_pipes(p);     // create p->pipefd
    for (int i = 0; i < p->num_commands; i++)
    {
        // Build the correct_path for the current command
        create_path_pipes(p, head, i); 
        // Now p->correct_path may be set (or NULL if not found)

        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0) // Child
        {
            // Depending on i, set up pipes
            if (i == 0)
                first_command(p, i);
            else if (i == p->num_commands - 1)
                last_command(p, i);
            else
                middle_commands(p, i);

            execute_command_pipes(p, head, i);
        }
        else // Parent
        {
            waitpid(pid, NULL, 0);
            // Close the previous pipe endpoints
            if (i > 0)
            {
                close(p->pipefd[i - 1][0]);
                close(p->pipefd[i - 1][1]);
            }
            // Free p->correct_path for this command
            // since we won't need it next iteration
            if (p->correct_path)
            {
                free(p->correct_path);
                p->correct_path = NULL;
            }
        }
    }
    free_pipe(p, p->nb_of_pipes);
    free_2d_array(p->store_pipe_arg);

    // If you had any leftover p->full_path_pipe, free it
    if (p->full_path_pipe)
    {
        free(p->full_path_pipe);
        p->full_path_pipe = NULL;
    }
}


void parse_pipe_args(t_data *p)
{
	parse_pipe_arg(p);
	create_pipe_arg(p);
	trim_pipe_args(p);
	p->num_commands = p->nb_of_pipes + 1;
}
