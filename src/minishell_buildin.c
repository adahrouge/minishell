/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_buildin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:24:42 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/13 12:59:58 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int pwd(t_data *p)
{
	p->pwd = getcwd(NULL, 0);
	if (!p->pwd)
	{
		perror("failed to allocate for pwd");
		return 1;
	}
    printf("%s\n", p->pwd);
    free(p->pwd);
	return 0;
}

int cd(t_data *p, int value)
{
	const char *home;

    if ((p->args == 1) || (p->args == 2 && 
        ft_strcmp(p->cmd_args[1], "~") == 0)) 
    {
       home = getenv("HOME");
        if (home == NULL) 
        {
            printf("HOME not set\n");
            return 1;
        }
        value = chdir(home);
    }
    else if (p->args == 2 && (ft_strcmp(p->cmd_args[1], 
        "..") == 0 || ft_strcmp(p->cmd_args[1], "-") == 0))
        value = chdir(p->cmd_args[1]);
    else if (p->args == 2) 
        value = chdir(p->cmd_args[1]);
    if (value == -1) 
    {
        perror("cd");
        return 1;
    }
    return 0;
}

void build_in(t_data *p, t_export **head)
{
    int value;
    value = 0;
	if ((ft_strcmp(p->cmd_args[0], "pwd") == 0) && (p->args == 1))
        p->exit_code = pwd(p);
	else if (ft_strcmp(p->cmd_args[0], "cd") == 0)
		p->exit_code = cd(p, value);
    else if (ft_strcmp(p->cmd_args[0], "echo") == 0 && (!p->cmd_args[1]))
    {
        write(1, "\n", 1);
        p->exit_code = 0;
        return ;
    }
    else if (ft_strcmp(p->cmd_args[0], "echo") == 0)
        p->exit_code = echo(p, *head);
     else if (ft_strcmp(p->cmd_args[0], "export") == 0)
        p->exit_code = export_main(p, head);
     else if (ft_strcmp(p->cmd_args[0], "env") == 0)
        p->exit_code = env(*head);
     else if (ft_strcmp(p->cmd_args[0], "unset") == 0)
        p->exit_code = ft_unset_all(p->input, head);
     else
     {
        printf("bash: %s: command not found\n", p->cmd_args[0]);
        p->exit_code = 127;
     }
}

