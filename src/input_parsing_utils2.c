/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing_utils2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 11:56:13 by adahroug          #+#    #+#             */
/*   Updated: 2025/03/06 14:03:30 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	input_contains_pipe(t_data *p)
{
	int	i;
	int	in_quotes;

	in_quotes = 0;
	i = 0;
	while (p->input[i] != '\0')
	{
		if (in_quotes == 0 && (p->input[i] == '\'' || p->input[i] == '"'))
			in_quotes = 1;
		else if (in_quotes == 1 && (p->input[i] == '\'' || p->input[i] == '"'))
			in_quotes = 0;
		else if (p->input[i] == '|' && in_quotes == 0)
			return (1);
		i++;
	}
	return (0);
}

int	pipe_input_correct(t_data *p)
{
	int	i;
	int	in_quotes;
	int	segment_len;

	i = 0;
	in_quotes = 0;
	segment_len = 0;
	if (p->input[0] == '|')
	{
		printf("zsh: parse error near `|'\n");
		return (0);
	}
	if (!input_check_pipe(p, &i, &in_quotes, &segment_len))
		return (0);
	if (segment_len == 0)
	{
		printf("zsh: parse error near '|'\n");
		return (0);
	}
	return (1);
}

int	input_check_pipe(t_data *p, int *i, int *in_quotes, int *segment_len)
{
	while (p->input[*i] != '\0')
	{
		if (*segment_len == 0 && p->input[*i] == '|')
		{
			printf("error double pipes\n");
			return (0);
		}
		if ((*in_quotes) == 0 && (p->input[*i] == '\'' || p->input[*i] == '"'))
			(*in_quotes) = 1;
		else if ((*in_quotes) == 1
			&& (p->input[*i] == '\'' || p->input[*i] == '"'))
			(*in_quotes) = 0;
		else if (p->input[*i] == '|' && (*in_quotes) == 0)
		{
			*segment_len = 0;
			(*i)++;
			continue ;
		}
		(*segment_len)++;
		(*i)++;
	}
	return (1);
}
