/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parsing_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 12:39:44 by adahroug          #+#    #+#             */
/*   Updated: 2025/02/07 15:30:08 by adahroug         ###   ########.fr       */
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
	int in_quotes;

	in_quotes = 0;
	i = 0;
	while (p->input[i] != '\0')
	{
		if (in_quotes == 0 && (p->input[i] == '\'' || p->input[i] == '"'))
			in_quotes = 1;
		else if (in_quotes == 1 && (p->input[i] == '\'' || p->input[i] == '"'))
			in_quotes = 0;
		else if (p->input[i] == '|' && in_quotes == 0)
			return 1;
		i++;
	}
	return 0;
}
int pipeInputCorrect(t_data *p)
{
	int i;
	int in_quotes;
	int segment_len;

	i = 0;
	in_quotes = 0;
	segment_len = 0;
	if (p->input[0] == '|')
	{
		printf("zsh: parse error near `|'\n");
		return 0;
	}
	if (!inputCheckPipe(p, &i, &in_quotes, &segment_len))
		return 0;
	if (segment_len == 0)
	{
		printf("zsh: parse error near '|'\n");
		return 0;
	}
	return 1;
}
int inputCheckPipe(t_data *p, int *i, int *in_quotes, int *segment_len)
{
	while (p->input[*i] != '\0')
	{
		if (*segment_len == 0 && p->input[*i] == '|')
		{
			printf("error double pipes\n");
			return 0;
		}
		if ((*in_quotes) == 0 && (p->input[*i] == '\'' || p->input[*i] == '"'))
			(*in_quotes) = 1;
		else if ((*in_quotes) == 1 && (p->input[*i] == '\'' || p->input[*i] == '"'))
			(*in_quotes) = 0;
		else if (p->input[*i] == '|' && (*in_quotes) == 0)
			{
				*segment_len = 0;
				(*i)++;
				continue;
			}
		(*segment_len)++;
		(*i)++;
	}
	return 1;
}
