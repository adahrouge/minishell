/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:46:27 by adahroug          #+#    #+#             */
/*   Updated: 2024/11/22 17:22:24 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
void dollar_and_quotations(char *arg)
{
   int i;
   char quote;
   
   i = 0;
   quote = arg[i];
   i++;
   while (arg[i] != '\0' && arg[i] != quote)
    {
        printf("%c", arg[i]);
        i++;
    }
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
    i = 1;
    while (arg[i] != '\0')
    {
        if (arg[i] == '"' || arg[i] == '\'')
        {
            dollar_and_quotations(&arg[i]);
            return 1;
        }
        i++;
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
int echo_with_n(t_data *p, int i)
{
    if (p->cmd_args[1] && (ft_strcmp(p->cmd_args[1], "-n") == 0
        || ft_strcmp(p->cmd_args[1], "\"-n\"") == 0))
    {
        i = 2;
        return i;
    }
    return 0;
}
int echo_wrong(t_data *p)
{
    if (p->cmd_args[1] == NULL)
    {
        write(1, "\n", 1);
        return 1;
    }
    return 0;
}
int echo(t_data *p)
{
    int i;
    int new_line;

    i = 1;
    new_line = 1;
    if (echo_with_n(p, i))
    {
        i = 2;
        new_line = 0;
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
