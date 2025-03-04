/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external_commands.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 10:10:30 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/28 12:45:27 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_external_commands(char **paths, char **new_paths)
{
	free_2d_array(paths);
	free_2d_array(new_paths);
}

void	execute_command(char *full_path, t_data *p, t_export *head)
{
	pid_t	pid;
	char	**envp;
	int		status;

	status = 0;
	envp = convert_list_to_array(head);
	pid = fork();
	if (pid == 0)
	{
		execve(full_path, p->cmd_args, envp);
		perror("execve failed");
		free(full_path);
		free_2d_array(envp);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		parent_execution(pid, status, p, full_path);
	else
	{
		free(full_path);
		p->exit_code = 1;
	}
	free_2d_array(envp);
}

void	parent_execution(pid_t pid, int status, t_data *p, char *full_path)
{
	if (waitpid(pid, &status, 0) == -1)
		p->exit_code = 1;
	else
		if (WIFEXITED(status))
			p->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		p->exit_code = 128 + WTERMSIG(status);
	free(full_path);
}

int	size_list(t_export *head)
{
	t_export	*ptr;
	int			i;

	ptr = head;
	i = 0;
	while (ptr != NULL)
	{
		ptr = ptr->next;
		i++;
	}
	return (i);
}

void	external_commands(t_data *p, t_export *head, char *path_env)
{
	char	**paths;
	char	**new_paths;
	char	*full_path;

	path_env = my_getenv("PATH", head);
	if (!path_env)
		return ;
	paths = ft_split(path_env, ':');
	new_paths = create_new_path(paths);
	if (!new_paths)
	{
		free_2d_array(paths);
		return ;
	}
	copy_paths(paths, new_paths);
	full_path = create_full_path(p, new_paths);
	if (full_path != NULL)
		execute_command(full_path, p, head);
	else
	{
		printf("bash: %s: command not found\n", p->cmd_args[0]);
		p->exit_code = 127;
	}
	free_external_commands(paths, new_paths);
	return ;
}
