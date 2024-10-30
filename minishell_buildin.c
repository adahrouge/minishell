/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_buildin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/30 09:24:42 by adahroug          #+#    #+#             */
/*   Updated: 2024/10/30 11:05:47 by adahroug         ###   ########.fr       */
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
// int cd(t_data *p)
// {
// 	int value;
// 	if (p->args == 2) //cd + a file name
// 		value = chdir(p->cmd_args[1]);
// 	if (value == -1 && p->cmd_args[1][0] == '/')
// 	{
// 		printf("%s",strerror(13));
// 		return 0;
// 	}
// 	else if (value == -1)
// 	{
// 		printf("%s",strerror(2));
// 		return 0;
// 	}
// 	if ((p->args == 2) && ((ft_strcmp(p->cmd_args[1], "..") == 0) || ft_strcmp(p->cmd_args[1], "-") == 0))  // cd + ..
// 		value = chdir(p->cmd_args[1]);
// 	if (getenv("HOME") == NULL)
// 	{
// 		printf("HOME not set");
// 		return 0;
// 	}
// 	if ((p->args == 1) || (p->args == 2 && ft_strcmp(p->cmd_args[1], "~") == 0))// cd ~ or cd alone
// 		value = chdir(getenv("HOME"));
// 	}
#include <stdio.h>
#include <unistd.h>  // for chdir, getenv
#include <errno.h>   // for errno
#include <string.h>  // for strerror

int cd(t_data *p)
{
    int value;
	const char *home;
	value = 0;
    // If the user enters 'cd' without arguments or 'cd ~'
    if ((p->args == 1) || (p->args == 2 && ft_strcmp(p->cmd_args[1], "~") == 0)) 
    {
       home = getenv("HOME");
        if (home == NULL) 
        {
            printf("HOME not set\n");
            return 0;
        }
        value = chdir(home);
    }
    // If the user enters 'cd ..' or 'cd -'
    else if (p->args == 2 && (ft_strcmp(p->cmd_args[1], "..") == 0 || ft_strcmp(p->cmd_args[1], "-") == 0))
        value = chdir(p->cmd_args[1]);
    else if (p->args == 2) 
        value = chdir(p->cmd_args[1]);
    if (value == -1) 
    {
        perror("cd");  // Prints the error message based on errno
        return 0;
    }
    return 1;  // Return 1 if the operation is successful
}

void build_in(t_data *p)
{
	if ((ft_strcmp(p->cmd_args[0], "pwd") == 0) && (p->args == 1))
	{
		p->pwd = pwd(p);
		printf("%s\n", p->pwd);
	}
	else if (ft_strcmp(p->cmd_args[0], "cd") == 0)
	{
		cd(p);
	}
}
void free_buildin(t_data *p)
{
	free(p->pwd);
}