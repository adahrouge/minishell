/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:02:00 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/20 19:36:04 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *create_path_pipes(t_data *p, t_export *head, int cmd_index)
{
    char *path_env;
    char **paths;
    char **new_paths;
    char *cmd_name;
    char *result_path = NULL;
    if (!p->store_pipe_arg[cmd_index] || p->store_pipe_arg[cmd_index][0] == '\0')
        return NULL;

    cmd_name = parse_command(p, cmd_index);
    if (!cmd_name || cmd_name[0] == '\0')
    {
        free(cmd_name); 
        return NULL;
    }
    path_env = my_getenv("PATH", head);
    if (!path_env)
    {
        free(cmd_name);
        return NULL;
    }
    paths = ft_split(path_env, ':');
    if (!paths)
    {
        free(cmd_name);
        return NULL;
    }
    new_paths = create_new_path(paths); 
    if (!new_paths)
    {
        free(cmd_name);
        free_2d_array(paths);
        return NULL;
    }
    copy_paths(paths, new_paths);

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

// get ls from ls - l
char *parse_command(t_data *p, int cmd_index)
{
    char *raw;
    char *cmd;
    raw = p->store_pipe_arg[cmd_index];
    if (!raw || !raw[0])
        return NULL;

    int len = 0;
    while (raw[len] && raw[len] != ' ')
        len++;
    
    cmd = strndup(raw, len);
    return cmd;
}


