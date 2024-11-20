/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_buildin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:24:42 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/20 17:04:01 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void pwd(t_data *p)
{
	p->pwd = getcwd(NULL, 0);
	if (!p->pwd)
	{
		perror("failed to allocate for pwd");
		return ;
	}
    printf("%s\n", p->pwd);
    free(p->pwd);
	return ;
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
            return 0;
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
        return 0;
    }
    return 1;
}

void build_in(t_data *p)
{
    int value;

    value = 0;
	if ((ft_strcmp(p->cmd_args[0], "pwd") == 0) && (p->args == 1))
        pwd(p);
	else if (ft_strcmp(p->cmd_args[0], "cd") == 0)
		cd(p, value);
    else if (ft_strcmp(p->cmd_args[0], "echo") == 0)
        echo(p);
     else if (ft_strcmp(p->cmd_args[0], "export") == 0)
        export_main();
     else if (ft_strcmp(p->cmd_args[0], "env") == 0)
        env();
     else if (ft_strcmp(p->cmd_args[0], "unset") == 0)
        unset(p->cmd_args[1]);
     else
        printf("bash: %s: command not found\n", p->cmd_args[0]);
}

