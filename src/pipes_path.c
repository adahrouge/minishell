/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:02:00 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/19 16:12:14 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/**
 * create_path_pipes:
 * 1. parse_command -> local cmd_name
 * 2. Try PATH directories + cmd_name
 * 3. If found, store in p->correct_path
 */
char *create_path_pipes(t_data *p, t_export *head, int cmd_index)
{
    // Quick sanity check
    if (!p->store_pipe_arg[cmd_index] || p->store_pipe_arg[cmd_index][0] == '\0')
        return NULL;

    // parse out the command name from store_pipe_arg[cmd_index]
    char *cmd_name = parse_command(p, cmd_index);
    if (!cmd_name || cmd_name[0] == '\0')
    {
        free(cmd_name); // in case it returned ""
        return NULL;
    }

    // get PATH and split
    char *path_env = my_getenv("PATH", head);
    if (!path_env)
    {
        free(cmd_name);
        return NULL;
    }
    char **paths = ft_split(path_env, ':');
    if (!paths)
    {
        free(cmd_name);
        return NULL;
    }
    char **new_paths = create_new_path(paths); 
    if (!new_paths)
    {
        free(cmd_name);
        free_2d_array(paths);
        return NULL;
    }
    copy_paths(paths, new_paths);

    // Try each path
    char *result_path = NULL;
    for (int j = 0; new_paths[j] != NULL; j++)
    {
        // +2 for safety (one for '/', one for '\0')
        size_t len_path = ft_strlen(new_paths[j]);
        size_t len_cmd  = ft_strlen(cmd_name);

        p->full_path_pipe = malloc(len_path + len_cmd + 2);
        if (!p->full_path_pipe)
            break; // out of memory => break and cleanup

        ft_strcpy(p->full_path_pipe, new_paths[j]);
        strcat(p->full_path_pipe, cmd_name);

        if (access(p->full_path_pipe, X_OK) == 0)
        {
            // Found a valid executable
            result_path = ft_strdup(p->full_path_pipe);
            free(p->full_path_pipe);
            p->full_path_pipe = NULL;
            break;
        }

        free(p->full_path_pipe);
        p->full_path_pipe = NULL;
    }
    // free everything
    free(cmd_name);
    free_2d_array(new_paths);
    free_2d_array(paths);

    // store or return
    if (result_path)
        p->correct_path = result_path; // keep for later use
    else
        p->correct_path = NULL;

    return p->correct_path;
}


char *create_full_path_pipe(t_data *p, char **new_paths, int *i)
{
    // Ensure the pipe segment is valid
    if (!p->store_pipe_arg[*i] || p->store_pipe_arg[*i][0] == '\0')
        return NULL;

    // Parse out just the command (e.g. "cat")
    if (p->command_name)
    {
        free(p->command_name);
        p->command_name = NULL;
    }
    char *command_name = parse_command(p, *i);
    if (!command_name || command_name[0] == '\0')
        return NULL;

    int j = 0;
    while (new_paths[j] != NULL)
    {
        size_t len_path = ft_strlen(new_paths[j]);
        size_t len_cmd  = ft_strlen(command_name);

        // +1 in case new_paths[j] doesn't already end with '/' 
        // (but your copy_paths usually appends '/', so +2 is extra safe).
        p->full_path_pipe = malloc((len_path + len_cmd + 2) * sizeof(char));
        if (!p->full_path_pipe)
        {
            free(command_name);
            return NULL;
        }
        ft_strcpy(p->full_path_pipe, new_paths[j]);
        strcat(p->full_path_pipe, command_name);

        // If accessible, we found the correct path
        if (access(p->full_path_pipe, X_OK) == 0)
        {
            p->correct_path = ft_strdup(p->full_path_pipe);
            free(p->full_path_pipe);
            p->full_path_pipe = NULL;
            free(command_name);
            return p->correct_path;
        }
        free(p->full_path_pipe);
        p->full_path_pipe = NULL;
        j++;
    }
    free(command_name);
    return NULL; // No valid path
}


/**
 * parse_command:
 * Takes store_pipe_arg[cmd_index] (e.g. "cat -e") 
 * and returns a newly allocated string for just the command portion ("cat").
 */
char *parse_command(t_data *p, int cmd_index)
{
    char *raw = p->store_pipe_arg[cmd_index];
    if (!raw || !raw[0])
        return NULL;

    // find where the first space occurs
    int len = 0;
    while (raw[len] && raw[len] != ' ')
        len++;

    // strndup the portion up to 'len'
    char *cmd = strndup(raw, len);
    // strndup returns malloc'd memory or NULL on failure
    return cmd;
}


