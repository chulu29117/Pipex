/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:07:41 by clu               #+#    #+#             */
/*   Updated: 2025/03/10 12:41:23 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	first_child(t_pipex *pipex, char **argv, char **envp);
static void	second_child(t_pipex *pipex, char **argv, char **envp);
static void	open_files(t_pipex *pipex, char **argv);

// Execute the pipex command
	// Fork the first child process (to exec cmd1)
	// Child: Run the first child process
	// Fork the second child process (to exec cmd2)
	// Child: Run the second child process
	// Parent: Close the read and write end of the pipe
	// Parent Wait for the first and second child processes to finish
	// Check and return the exit status of the second child process
int	exec_pipex(t_pipex *pipex, char **argv, char **envp)
{
	int		status1;
	int		status2;
	pid_t	pid1;
	pid_t	pid2;

	open_files(pipex, argv);
	if (pipe(pipex->pipe_fds) == -1)
		ft_pipex_error("pipex: pipe creation failed", 1);
	pid1 = fork();
	if (pid1 == -1)
		close_fd(pipex);
	if (pid1 == 0)
		first_child(pipex, argv, envp);
	pid2 = fork();
	if (pid2 == -1)
		close_fd(pipex);
	if (pid2 == 0)
		second_child(pipex, argv, envp);
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	waitpid(pid1, &status1, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (1);
}

static void	open_files(t_pipex *pipex, char **argv)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		perror(argv[1]);
	}
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		perror(argv[4]);
	}
}

// First child process
	// Open the input file for reading
	// Redirect the standard input to the input file
	// Redirect the standard output to the write end of the pipe
	// Close the input file and the read end of the pipe
	// Execute the first command
static void	first_child(t_pipex *pipex, char **argv, char **envp)
{
	if (pipex->infile == -1)
		exit(1);
	dup2(pipex->pipe_fds[1], STDOUT_FILENO);
	dup2(pipex->infile, STDIN_FILENO);
	close(pipex->infile);
	close(pipex->pipe_fds[0]);
	exec_cmd(argv[2], envp);
}

// Second child process
	// Open the output file for writing	
	// Redirect the standard input to the read end of the pipe
	// Redirect the standard output to the output file
	// Close the output file and the write end of the pipe
	// Execute the second command
static void	second_child(t_pipex *pipex, char **argv, char **envp)
{
	if (pipex->outfile == -1)
		exit(1);
	dup2(pipex->pipe_fds[0], STDIN_FILENO);
	dup2(pipex->outfile, STDOUT_FILENO);
	close(pipex->outfile);
	close(pipex->pipe_fds[1]);
	exec_cmd(argv[3], envp);
}
