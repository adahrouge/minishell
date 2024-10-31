/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouraad <abouraad@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 09:03:33 by adahroug          #+#    #+#             */
/*   Updated: 2024/10/31 13:27:43 by abouraad         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <linux/limits.h>

void read_command_line(t_data *p)
{
	p->args = count_words(p->input, ' ');
	p->cmd_args = ft_split(p->input, ' ');
}

void free_allocated(t_data *p)
{
	int i;
	i = 0;
	while (i < p->args)
	{
		free(p->cmd_args[i]);
		i++;
	}
	free(p->cmd_args);
	free(p->input);
	free(p);
}
void loop(t_data *p)
{
    while (1)
    {
        p->input = readline("minishell > ");
        if (p->input == NULL)
        {
            write(1, "exit\n", 5);
            break;
        }
        if (p->input[0] == '\0')
        {
            free(p->input);
            continue;
        }
        if (strcmp(p->input, "exit") == 0)
        {
            free(p->input);
            break;
        }
        add_history(p->input);
        read_command_line(p);
        build_in(p);
        free(p->input);
    }
}
int main(int argc, char **argv)
{
	t_data *p;
	
	signal(SIGINT, sigint_handler);
    	signal(SIGQUIT, sigquit_handler);
	if (argc != 1 && ft_strcmp(argv[0], "./minishell") != 0)
		return 0;
	p = malloc(sizeof(t_data));
	if (!p)
		return 0;
	loop(p);
	free(p);
	return (0);
}
