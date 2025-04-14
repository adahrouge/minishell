/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:37:04 by adahroug          #+#    #+#             */
/*   Updated: 2025/04/14 15:17:13 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_slash(char *arg)
{
	int	i;

	i = 0;
	if (!arg)
		return (0);
	while (arg[i] != '\0')
	{
		if (arg[i] == '/')
			return (1);
		i++;
	}
	return (0);
}

void	update_shlvl(t_export **head)
{
	char	*old;
	int		lvl;
	char	*arg;
	char	*temp;

	old = my_getenv("SHLVL", *head);
	if (!old)
		lvl = 1;
	else
	{
		lvl = atoi(old);
		if (lvl < 0)
			lvl = 0;
		lvl++;
	}
	temp = ft_itoa(lvl);
	arg = ft_strjoin("SHLVL=", temp);
	free(temp);
	process_export_args(arg, head);
	free(arg);
}

void	executable(t_data *p, t_export *head)
{
	char	**envp;
	char	*cmd;

	cmd = p->cmd_args[0];
	envp = convert_list_to_array(head);
	if (has_slash(p->cmd_args[0]))
	{
		if (access(p->cmd_args[0], X_OK) == 0)
			execute_command(ft_strdup(p->cmd_args[0]), p, head);
		else if (access(cmd, F_OK) != 0)
		{
			executable_error(p, cmd, envp);
			return ;
		}
		else if (access(cmd, X_OK) != 0)
		{
			executable_error(p, cmd, envp);
			return ;
		}
		else
			execve(cmd, p->cmd_args, envp);
	}
	free_2d_array(envp);
}

void	executable_error(t_data *p, char *cmd, char **envp)
{
	printf("No such file: %s\n", cmd);
	p->exit_code = 127;
	free_2d_array(envp);
	return ;
}

void	executable_main(t_data *p, t_export **head)
{
	executable(p, *head);
	free_split(p->cmd_args);
	free(p->input);
	return ;
}
