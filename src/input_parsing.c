/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/01 08:43:23 by adahroug          #+#    #+#             */
/*   Updated: 2025/03/06 15:20:58 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_is_null(t_data *p)
{
	if (p->input == NULL)
	{
		write(1, "exit\n", 5);
		free(p->input);
		return (1);
	}
	return (0);
}

int	input_is_backslash(t_data *p)
{
	if (p->input[0] == '\0')
	{
		free(p->input);
		return (1);
	}
	return (0);
}
	
int	input_is_space(t_data *p)
{
	int	i;

	i = 0;
	while (p->input[i] != '\0')
	{
		if (p->input[i] == ' ')
			i++;
		else
			return (0);
	}
	free(p->input);
	return (1);
}

int	input_is_redirect(t_data *p)
{
	if ((ft_strncmp(p->input, ">>>>>>", 3) == 0)
		|| ft_strncmp(p->input, "<<<<<<", 4) == 0)
	{
		write(1, "bash: syntax error near unexpected token `>'\n", 45);
		free(p->input);
		return (1);
		p->exit_code = 2;
	}
	else if (ft_strcmp(p->input, "><") == 0)
	{
		write(1, "bash: syntax error near unexpected token `<'\n", 45);
		free(p->input);
		p->exit_code = 2;
		return (1);
	}
	else if (p->input[0] == '>' || p->input[0] == '<'
		|| ft_strcmp(p->input, "<<") == 0
		|| ft_strcmp(p->input, ">>") == 0 || ft_strcmp(p->input, "<>") == 0)
	{
		write(1, "bash: syntax error near unexpected token `newline'\n", 51);
		free(p->input);
		p->exit_code = 2;
		return (1);
	}
	return (0);
}
