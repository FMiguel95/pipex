/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/28 00:01:22 by fernacar          #+#    #+#             */
/*   Updated: 2023/08/07 22:18:56 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	free_split(char **split_ptr)
{
	int	i;

	i = 0;
	while (split_ptr[i])
	{
		free(split_ptr[i]);
		i++;
	}
	free(split_ptr);
}

char	**get_exec_paths(char **envp)
{
	char	**var_split;
	char	**path_split;

	while (*envp)
	{
		var_split = ft_split(*envp, '=');
		if (ft_strcmp(var_split[0], "PATH") == 0)
		{
			path_split = ft_split(var_split[1], ':');
			free_split(var_split);
			return (path_split);
		}
		free_split(var_split);
		envp++;
	}
	return (NULL);
}

char	*get_cmd_path(char **exe_paths, char *cmd)
{
	char	*cmd_path;
	char	*temp;

	cmd_path = NULL;
	while (*exe_paths)
	{
		temp = ft_strjoin(*exe_paths, "/");
		cmd_path = ft_strjoin(temp, cmd);
		free(temp);
		if (access(cmd_path, X_OK) == 0)
			break ;
		free(cmd_path);
		cmd_path = NULL;
		exe_paths++;
	}
	if (!cmd_path)
		cmd_path = ft_strdup(cmd);
	return (cmd_path);
}

void	error_exit(char *file)
{
	perror(file);
	exit(1);
}

void	error_cleaning(char *file, char *cmd, char **cmd_args)
{
	free(cmd);
	free_split(cmd_args);
	error_exit(file);
}
