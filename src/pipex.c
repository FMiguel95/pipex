/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:17:58 by fernacar          #+#    #+#             */
/*   Updated: 2023/08/08 02:21:03 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex.h"

void	error_cleaning(char *file, char *cmd, char **cmd_args)
{
	perror(file);
	free(cmd);
	free_split(cmd_args);
	exit(1);
}

void	first_command(int pipe[2], char **av, char **envp)
{
	char	*cmd;
	char	**cmd_args;
	char	**paths;
	int		fd;

	paths = get_exec_paths(envp);
	cmd_args = ft_split(av[2], ' ');
	cmd = get_cmd_path(paths, cmd_args[0]);
	free_split(paths);
	fd = open(av[1], O_RDONLY);
	if (fd < 0)
		error_cleaning(av[1], cmd, cmd_args);
	dup2(fd, STDIN_FILENO);
	close(fd);
	close(pipe[0]);
	dup2(pipe[1], STDOUT_FILENO);
	close(pipe[1]);
	if (execve(cmd, cmd_args, envp) == -1)
		error_cleaning(av[2], cmd, cmd_args);
}

void	last_command(int pipe[2], char **av, char **envp)
{
	char	*cmd;
	char	**cmd_args;
	char	**paths;
	int		fd;

	paths = get_exec_paths(envp);
	cmd_args = ft_split(av[3], ' ');
	cmd = get_cmd_path(paths, cmd_args[0]);
	free_split(paths);
	close(pipe[1]);
	dup2(pipe[0], STDIN_FILENO);
	close(pipe[0]);
	fd = open(av[4], O_CREAT | O_WRONLY | O_TRUNC, 0666);
	if (fd < 0)
		error_cleaning(av[4], cmd, cmd_args);
	dup2(fd, STDOUT_FILENO);
	close(fd);
	if (execve(cmd, cmd_args, envp) == -1)
		error_cleaning(av[3], cmd, cmd_args);
}

int	main(int ac, char **av, char **envp)
{
	int		pipe_fd[2];
	int		pid;

	if (ac != 5)
	{
		ft_printf("Usage: ./pipex <file1> <cmd1> <cmd2> <file2>\n");
		return (1);
	}
	else
	{
		pipe(pipe_fd);
		pid = fork();
		if (pid == 0)
		{
			first_command(pipe_fd, av, envp);
		}
		else
		{
			waitpid(pid, 0, 0);
			last_command(pipe_fd, av, envp);
		}
	}
	return (1);
}
