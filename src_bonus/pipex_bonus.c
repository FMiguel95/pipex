/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fernacar <fernacar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/26 18:17:58 by fernacar          #+#    #+#             */
/*   Updated: 2023/08/08 01:54:44 by fernacar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/pipex_bonus.h"

void	execute_command(char *arg, char **envp)
{
	char	*cmd;
	char	**cmd_args;
	char	**paths;

	paths = get_exec_paths(envp);
	cmd_args = ft_split(arg, ' ');
	cmd = get_cmd_path(paths, cmd_args[0]);
	free_split(paths);
	if (execve(cmd, cmd_args, envp) == -1)
		error_cleaning(arg, cmd, cmd_args);
}

void	pipe_loop(int ac, char **av, char **envp)
{
	pid_t	pid;
	int		pipe_fd[2];
	int		i;

	i = 1;
	while (++i < ac - 2)
	{
		if (pipe(pipe_fd) == -1)
			error_exit("pipe");
		pid = fork();
		if (pid == -1)
			error_exit("fork");
		if (pid == 0)
		{
			close(pipe_fd[0]);
			waitpid(pid, 0, 0);
			dup2(pipe_fd[1], STDOUT_FILENO);
			close(pipe_fd[1]);
			execute_command(av[i], envp);
		}
		close(pipe_fd[1]);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}

int	main(int ac, char **av, char **envp)
{
	int		fd_in;
	int		fd_out;

	if (ac < 5)
		ft_printf("Usage: ./pipex <infile> <cmd> ... <cmd> <outfile>\n\
or ./pipex here_doc <LIMITER> <cmd1> <cmd2> <outfile>");
	else if (ft_strcmp(av[1], "here_doc") == 0 && ac == 6)
		here_doc(ac, av, envp);
	else
	{
		fd_in = open(av[1], O_RDONLY);
		if (fd_in < 0)
			error_exit(av[1]);
		dup2(fd_in, STDIN_FILENO);
		close(fd_in);
		pipe_loop(ac, av, envp);
		fd_out = open(av[ac - 1], O_CREAT | O_WRONLY | O_TRUNC, 0666);
		if (fd_out < 0)
			error_exit(av[ac - 1]);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		execute_command(av[ac - 2], envp);
	}
	return (1);
}
