/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executable.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 17:37:04 by adahroug          #+#    #+#             */
/*   Updated: 2025/03/08 18:08:41 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int has_slash(char *arg)
{
	int i;

	i = 0;
	while (arg[i] != '\0')
	{
		if (arg[i] == '/')
			return (1);
		i++;
	}
	return (0);
}
void update_shlvl(t_export **head)
{
	char *old;
	int lvl;
	char *arg;
	char *temp;

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
void executable(t_data *p, t_export *head)
{
	char **envp;
	char *cmd;

	cmd = p->cmd_args[0];
	envp = convert_list_to_array(head);
	if (has_slash(p->cmd_args[0]))
	{
		if (access(cmd, F_OK) != 0)
		{
			write(2, "No such file: ", 14);
			write(2, cmd, ft_strlen(cmd));
			write(2, "\n", 1);
			p->exit_code = 127;
			exit(127);
		}
		if (access(cmd, X_OK) != 0)
		{
			write(2, "Permission denied: ", 19);
			write(2, cmd, ft_strlen(cmd));
			write(2, "\n", 1);
			exit(126);
		}
		execve(cmd, p->cmd_args, envp);
		perror("excve executable");
		exit(126);
	}
}
