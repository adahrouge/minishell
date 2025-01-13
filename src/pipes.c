/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 13:19:11 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/13 14:45:47 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void minishell_pipes(t_data *p)
{
	char **commands;
	int num_commands;

	num_commands = 0;
	commands = parse_pipes(p);
	while (commands[num_commands] != '\0')
		num_commands++;
	
}
char **parse_pipes(t_data *p)
{
	int i = 0;
	char **commands;
	
	commands = ft_split(p->input[i], '|');
	if (!commands)
		return NULL;
	return commands;
}
