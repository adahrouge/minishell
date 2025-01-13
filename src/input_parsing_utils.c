/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:39:44 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/13 13:18:25 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int input_is_slash(t_data *p)
{
	if (p->input[0] == '/')
	{
		printf("bash: %s: Is a directory\n", p->input);
		p->exit_code = 126;
		free(p->input);
		return 1;
	}
	return 0;
}
int input_is_dash(t_data *p)
{
	if (p->input[0] == '-')
	{
		write(1, "-: command not found\n", 21);
		p->exit_code = 127;
		free(p->input);
		return 1;
	}
	return 0;
}
int input_is_and(t_data *p)
{
	if (ft_strncmp(p->input, "&&&&", 2) == 0)
	{
		printf("bash: syntax error near unexpected token `&&'\n");
		p->exit_code = 2;
		free(p->input);
		return 1;
	}
	if (p->input[0] == '&')
	{
		printf("bash: syntax error near unexpected token `&'\n");
		p->exit_code = 2;
		free(p->input);
		return 1;
	}
	return 0;
}
int input_is_makefile(t_data *p)
{
	if (ft_strncmp(p->input, "Makefile", 8) == 0 ||
		ft_strncmp(p->input, "makefile", 8) == 0)
	{
		printf("bash: %s: command not found\n", p->input);
		free(p->input);
		return 1;
	}
	return 0;
}
int input_is_clear(t_data *p)
{
	if (ft_strcmp(p->input, "clear") == 0)
	{
		printf("\033[H\033[J");
		free(p->input);
		return 1;
	}
		return 0;
}

int input_contains_pipe(t_data *p)
{
	int i;
	int counter;

	counter = 0;
	i = 0;
	while (p->input[i] != '\0')
	{
		if (p->input[i] == '|')
			counter++;
		i++;
	}
	if (counter > 0)
		return 1;
	return 0;
}
