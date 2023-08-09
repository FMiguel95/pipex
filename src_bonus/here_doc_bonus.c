/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/08 01:52:57 by fernacar          #+#    #+#             */
/*   Updated: 2023/08/08 02:00:57 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

static void	here_doc_get(char **av, int pipe[2])
{
	char	*limiter;
	char	*line;

	limiter = ft_strjoin(av[2], "\n");
	while (1)
	{
		line = get_next_line(STDIN_FILENO);
		if (!line || ft_strcmp(line, limiter) == 0)
		{
			if (line)
				free(line);
			break ;
		}
		ft_putstr_fd(line, pipe[1]);
		free(line);
	}
	free(limiter);
	get_next_line(-1);
}

static void	here_doc_child(int pipe_fd[2][2], char **av, char **envp)
{
	pipe(pipe_fd[0]);
	if (pipe(pipe_fd[0]) == -1)
		error_exit("pipe");
	here_doc_get(av, pipe_fd[0]);
	close(pipe_fd[0][1]);
	dup2(pipe_fd[0][0], STDIN_FILENO);
	close(pipe_fd[0][0]);
	dup2(pipe_fd[1][1], STDOUT_FILENO);
	close(pipe_fd[1][1]);
	close(pipe_fd[1][0]);
	execute_command(av[3], envp);
}

void	here_doc(int ac, char **av, char **envp)
{
	pid_t	pid;
	int		pipe_fd[2][2];
	int		fd_out;

	pipe(pipe_fd[1]);
	if (pipe(pipe_fd[1]) == -1)
		error_exit("pipe");
	pid = fork();
	if (pid == -1)
		error_exit("pid");
	if (pid == 0)
		here_doc_child(pipe_fd, av, envp);
	waitpid(pid, 0, 0);
	close(pipe_fd[1][1]);
	dup2(pipe_fd[1][0], STDIN_FILENO);
	close(pipe_fd[1][0]);
	fd_out = open(av[ac - 1], O_CREAT | O_WRONLY | O_APPEND, 0666);
	if (fd_out < 0)
		error_exit(av[ac - 1]);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_out);
	execute_command(av[ac - 2], envp);
}
