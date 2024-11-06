/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:46:27 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/06 13:31:26 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_if_dollar(t_data *p)
{
    if (p->cmd_args[1][0] == '$')
        return 1;
    return 0;
}
int set_up_tmp(t_data *p)
{
    int i;
    char *tmp;
    char *value;
    i = 0;
    tmp = malloc(ft_strlen(p->cmd_args[1]) + 1);
    if (!tmp)
        return 0;
    ft_strcpy(tmp, p->cmd_args[1]);
    while (i < ft_strlen(tmp))
    {
        tmp[i] = tmp[i + 1];
        i++;
    }
    value = getenv(tmp);
    if (value != NULL)
        printf("%s", value);
    free(tmp);
    return 0;
}
int echo_dollar(t_data *p)
{
    set_up_tmp(p);
    if (ft_strcmp(p->cmd_args[1],"$?") == 0)
         printf("0");
    else if (ft_strcmp(p->cmd_args[1], "$?$") == 0)
        printf("0$");
    else
        printf("\n");
    return 1;
}

int write_echo(t_data *p, int i)
{
    int j;
    while (p->cmd_args[i] != NULL)
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
    return 1;
}
int echo(t_data *p, int i)
{
    i = 1;
    if (echo_wrong(p))
        return 0;
    if (check_if_dollar(p))
    {
    echo_dollar(p);
    return 0;
    }
    if (ft_strcmp(p->cmd_args[1], "-n") == 0 ||
        ft_strcmp(p->cmd_args[1], "\"-n\"") == 0)
        i = 2;
    write_echo(p, i);
    if (ft_strcmp(p->cmd_args[1], "-n") == 0 ||
        ft_strcmp(p->cmd_args[1], "\"-n\"") == 0)
        return 0;
    write(1, "\n", 1);
    return 1;
}