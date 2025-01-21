/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:02:00 by adahroug          #+#    #+#             */
/*   Updated: 2025/01/21 18:54:21 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_cmd_name_valid(t_data *p, int cmd_index, char **cmd_name)
{
	*cmd_name = NULL;
    if (!p->store_pipe_arg[cmd_index]
        || p->store_pipe_arg[cmd_index][0] == '\0')
        return 0;

    *cmd_name = parse_command(p, cmd_index);

    if (!*cmd_name || (*cmd_name)[0] == '\0')
    {
        free(*cmd_name);
        return 0;
    }
    return 1;
}
char **build_paths_and_newpaths(char *path_env, char *cmd_name)
{
    char **paths;
    char **new_paths;

    paths = NULL;
    new_paths = NULL;

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
    free_2d_array(paths);
    return new_paths;
}
char *search_executable(t_data *p, char **new_paths, char *cmd_name)
{
    char *result_path;
    int j;

    result_path = NULL;
    j = 0;

    while (new_paths[j] != NULL)
    {
        if (!build_full_path(p, new_paths[j], cmd_name))
            break;
        if (access(p->full_path_pipe, X_OK) == 0)
        {
            result_path = ft_strdup(p->full_path_pipe);
            free(p->full_path_pipe);
            p->full_path_pipe = NULL;
            break;
        }
        free(p->full_path_pipe);
        p->full_path_pipe = NULL;
        j++;
    }
    return result_path;
}
int build_full_path(t_data *p, char *dir, char *cmd)
{
    size_t len_path;
    size_t len_cmd;

    len_path = ft_strlen(dir);
    len_cmd = ft_strlen(cmd);

    p->full_path_pipe = malloc(len_path + len_cmd + 2);
    if (!p->full_path_pipe)
        return 0;

    ft_strcpy(p->full_path_pipe, dir);
    strcat(p->full_path_pipe, cmd);
    return 1;
}
char *create_path_pipes(t_data *p, t_export *head, int cmd_index)
{
    char *cmd_name;
    char *path_env;
    char **new_paths;
    char *result_path;

    new_paths = NULL; 
    result_path = NULL;
    if (!check_cmd_name_valid(p, cmd_index, &cmd_name))
        return NULL;
    path_env = my_getenv("PATH", head);
    if (!path_env)
    {
        free(cmd_name);
        return NULL;
    }
    new_paths = build_paths_and_newpaths(path_env, cmd_name);
    if (!new_paths)
        return NULL;
    result_path = search_executable(p, new_paths, cmd_name);
    free(cmd_name);
    free_2d_array(new_paths);
    if (result_path)
        p->correct_path = result_path;
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


