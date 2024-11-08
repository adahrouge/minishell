/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:46:27 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/08 19:43:40 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// int check_if_dollar(t_data *p)
// {
//     int i;
//     i = 0;
    
//     while (p->input[i] != '\0')
//     {
//         if (p->input[i] == '$')
//         {
//             echo_dollar(p, i);
//             return 1;
//         }
//         i++;
//     }
//     return 0;
// }
// int set_up_tmp(t_data *p)
// {
//     int i;
//     char *tmp;
//     char *value;
//     i = 0;
//     tmp = malloc(ft_strlen(p->cmd_args[1]) + 1);
//     if (!tmp)
//         return 0;
//     ft_strcpy(tmp, p->cmd_args[1]);
//     while (i < ft_strlen(tmp))
//     {
//         tmp[i] = tmp[i + 1];
//         i++;
//     }
//     value = getenv(tmp);
//     if (value != NULL)
//         printf("%s", value);
//     free(tmp);
//     return 0;
// }
// int print_echo_with_dollar(t_data *p)
// {
//     int j;
//     int x;
//     int len;
//     char *tmp;
//     char *value;
    
//     j = 0;
//     while (p->input[j] != '$')
//         j++;
//     if (p->input[j] == '$')
//         {
//             x = j + 1;
//  while ((p->input[x] >= 'A' && p->input[x] <= 'Z') || 
//             (p->input[x] >= 'a' && p->input[x] <= 'z') || 
//             (p->input[x] >= '0' && p->input[x] <= '9') || p->input[x] == '_')
//             x++;
//         }
//     len = x - (j + 1);
//     tmp = ft_substr(p->input, j + 1, len);
//         value = getenv(tmp);
//     if (value != NULL)
//         printf("%s", value);
//     free(tmp);
//     print_remaining(p, x);
//     return 0;   
// }
// int print_remaining(t_data *p, int x)
// {
//     while (p->input[x] != '\0' && p->input[x] != ' ')
//     {
//         printf("%c", p->input[x]);
//         x++;
//     }
//    return 0;
// }
// int echo_dollar(t_data *p, int location)
// {
//     int i;
//     i = 5;
//     if (location == 5)
//     {
//     set_up_tmp(p);
//     printf("\n");
//     return 1;   
//     }
//     else
//     {
//         while (i < location)
//         {
//             printf("%c", p->input[i]);
//             i++;
//         }
//         print_echo_with_dollar(p);
//         printf("\n");
//     }
//     return 0;
// }

// int write_echo(t_data *p, int i)
// {
//     int j;
//     while (p->cmd_args[i] != NULL)
//     {
//         j = 0;
//         while(p->cmd_args[i][j] != '\0')
//         {
//             write(1, &p->cmd_args[i][j], 1);
//         j++;
//         }
//         if (p->cmd_args[i + 1] != NULL)
//             write(1, " ", 1);
//         i++;
//     }
//     return 1;
// }
// int echo(t_data *p, int i)
// {
//     i = 1;
//     if (echo_wrong(p))
//         return 0;
//     if (check_if_dollar(p))
//     return 0;
//     if (ft_strcmp(p->cmd_args[1], "-n") == 0 ||
//         ft_strcmp(p->cmd_args[1], "\"-n\"") == 0)
//         i = 2;
//     write_echo(p, i);
//     if (ft_strcmp(p->cmd_args[1], "-n") == 0 ||
//         ft_strcmp(p->cmd_args[1], "\"-n\"") == 0)
//         return 0;
//     write(1, "\n", 1);
//     return 1;
// }


//new:

void echo_count_quotes(t_data *p, int *double_quotes, int *single_quotes)
{
    int i;

    *double_quotes = 0;
    *single_quotes = 0;
    i = 0;
    while (p->input[i] != '\0')
    {
        if (p->input[i]  ==  '"')
            (*double_quotes)++;
        if (p->input[i] == '\'')
            (*single_quotes)++;
        i++;
    }
}
int parse_echo_with_quotes(t_data *p)
{
    int double_quotes;
    int single_quotes;
 
    echo_count_quotes(p, &double_quotes, &single_quotes);
    if (double_quotes % 2 != 0 || single_quotes % 2 != 0)
    {
        printf("bash : missing quotes\n");
        return 0;
    }
    return 1;
    }

int check_echo_input(t_data *p)
{
    int i;
    int j;

    j = 0;
    i = 0;
    while (p->cmd_args[i] != NULL)
    {
    j = 0;
    while (p->cmd_args[i][j] != '\0')
    {
        if (p->cmd_args[i][j] == '<' || p->cmd_args[i][j] == '|')
        {
        printf("minishell: syntax error near unexpected token : %c\n", p->cmd_args[i][j]);
        return 0;
        }
        if (p->cmd_args[i][j] == '"' || p->cmd_args[i][j] == '\'')
        {
            if (!parse_echo_with_quotes(p))
            return 0;
        }
        j++;
    }
    i++;
    }
    return 1;
}

int echo(t_data *p)
{
    int i;
    int new_line;

    i = 1;
    new_line = 1;
    if (p->cmd_args[1] && (ft_strcmp(p->cmd_args[1], "-n") == 0
        || ft_strcmp(p->cmd_args[1], "\"-n\"") == 0))
    {
        new_line = 0;
        i = 2;
    }
    if (!check_echo_input(p))
        return 0;
    while (p->cmd_args[i])
    {
        process_argument(p->cmd_args[i], p);
        if (p->cmd_args[i + 1])
            printf(" ");
        i++;
    }
    if (new_line)
        printf("\n");
    return 1;
}
int check_if_nb(char *arg)
{
    int i;
    i = 1;
    
    if (arg[i] > '0' && arg[i] <= '9')
    {
        i++;
        while(arg[i] != '\0')
        {
            printf("%c", arg[i]);
            i++;
        }
        return 1;
    }
    return 0;
}

void process_argument(char *arg, t_data *p)
{
    int i;
    int consumed;
    i = 0;
    while (arg[i])
    {
        if (arg[i] == '$')
        {
        if (check_if_nb(arg))
            return ;
        consumed = expand_variable(&arg[i], p);
            if (consumed == -1)
            {
            newline_loop();
            return;
            }
        i = i + consumed;
        }
        else if (arg[i] == '"' || arg[i] == '\'')
            i++;
        else
        {
            printf("%c", arg[i]);
            i++;
        }
    }
}
void newline_loop(void)
{
    printf("\033[2K");
    printf("\033[1G"); 
}
int expand_variable(char *str, t_data *p)
{
    int i;
    char var_name[256];
    int var_len;
    
    var_len = 0;
    i = 1;
    while ((str[i] >= 'A' && str[i] <= 'Z') ||
           (str[i] >= 'a' && str[i] <= 'z') ||
           (str[i] >= '0' && str[i] <= '9') ||
            str[i] == '_')
        var_name[var_len++] = str[i++];
    var_name[var_len] = '\0';
    p->value = getenv(var_name);
    if (p->value)
        printf("%s", p->value);
        else if (!p->value)
        return -1;
    return i;
}
