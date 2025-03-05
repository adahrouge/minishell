/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 17:26:36 by adahroug          #+#    #+#             */
/*   Updated: 2025/03/05 17:42:57 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void expand_all_tokens(char **tokens, t_export *head)
{
	int i;

	i = 0;
	while (tokens[i] != NULL)
	{
		tokens[i] = expand_single_token(tokens[i], head);
		i++;
	}
}
char *expand_single_token(char *oldtoken, t_export *head)
{
	char *newstr;
	int i;
	int dest;
	int len;

	len = ft_strlen(oldtoken);
	newstr = malloc(len * 8 + 1);
	if (!newstr)
		return oldtoken;
	i = 0;
	dest = 0;
	while (oldtoken[i] != '\0')
	{
		if (oldtoken[i] == '$')
			handle_expansion(oldtoken, &i, newstr, &dest, head);
		else
		{
			newstr[dest] = oldtoken[i];
			dest++;
			i++;
		}
	}
	newstr[dest] = '\0';
	free(oldtoken);
	return (newstr);
}
char *handle_expansion(char *oldtoken, int *i_ptr, char *newstr, int *dest_ptr, t_export *head)
{
	char var_name[256];
	char *value;
	int i;
	int dest;
	int var_len;

	i = *i_ptr;
	dest = *dest_ptr;
	i++;
	var_len = parse_var_name(oldtoken, &i, var_name);
	if (var_len > 0)
	{
		value = my_getenv(var_name, head);
		if (value)
		dest = copy_value(newstr, dest, value);
	}
	*i_ptr = i;
	*dest_ptr = dest;
	return (newstr);
}
int copy_value(char *dest_str, int dest, char *value)
{
	int k;

	k = 0;
	while (value[k] != '\0')
	{
		dest_str[dest] = value[k];
		dest++;
		k++;
	}
	return dest;
}
int parse_var_name(char *old, int *i_ptr, char *var_name)
{
    int var_len;

    var_len = 0;
    while (old[*i_ptr] != '\0'
        && ((old[*i_ptr] >= 'a' && old[*i_ptr] <= 'z')
            || (old[*i_ptr] >= 'A' && old[*i_ptr] <= 'Z')
            || (old[*i_ptr] >= '0' && old[*i_ptr] <= '9')
            || old[*i_ptr] == '_'))
    {
        var_name[var_len] = old[*i_ptr];
        var_len++;
        (*i_ptr)++;
    }
    var_name[var_len] = '\0';
    return (var_len);
}
