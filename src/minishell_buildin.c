/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_buildin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouraad <abouraad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:24:42 by adahroug          #+#    #+#             */
/*   Updated: 2024/10/31 13:23:58 by abouraad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *pwd(t_data *p)
{
	p->pwd = getcwd(NULL, 0);
	if (!p->pwd)
	{
		perror("failed to allocate for pwd");
		return NULL;
	}
	return p->pwd;
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
int echo(t_data *p)
{
    int i;
    int j;
    i = 1;
    if (p->cmd_args[1] == NULL)
    {
        write(1, "\n", 1);
        return 0;
    }
    if (ft_strcmp(p->cmd_args[1], "-n") == 0)
        i = 2;
    while(p->cmd_args[i] != NULL)
    {
        j = 0;
        while(p->cmd_args[i][j] != '\0')
        {
            write(1, &p->cmd_args[i][j], 1);
        j++;
        }
        if (p->cmd_args[i + 1] != NULL)
            write(1, " ", 1);
        i++;
    }
    if (ft_strcmp(p->cmd_args[1], "-n") == 0)
    {
        return 0;
    }
    write(1, "\n", 1);
    return 1;
}

void build_in(t_data *p)
{
    int value;
    value = 0;
	if ((ft_strcmp(p->cmd_args[0], "pwd") == 0) && (p->args == 1))
	{
		p->pwd = pwd(p);
		printf("%s\n", p->pwd);
        free(p->pwd);
	}
	else if (ft_strcmp(p->cmd_args[0], "cd") == 0)
		cd(p, value);
    else if (ft_strcmp(p->cmd_args[0], "echo") == 0)
        echo(p);
}
