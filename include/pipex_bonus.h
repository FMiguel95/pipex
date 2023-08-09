/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:20:34 by fernacar          #+#    #+#             */
/*   Updated: 2023/08/08 01:54:48 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "../libft/libft.h"
# include <sys/types.h>
# include <wait.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <errno.h>

void	free_split(char **split_ptr);
char	**get_exec_paths(char **envp);
char	*get_cmd_path(char **exe_paths, char *cmd);
void	error_exit(char *file);
void	error_cleaning(char *file, char *cmd, char **cmd_args);
void	execute_command(char *arg, char **envp);
void	here_doc(int ac, char **av, char **envp);

#endif