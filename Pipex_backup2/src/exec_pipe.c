/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:07:41 by clu               #+#    #+#             */
/*   Updated: 2025/02/25 22:17:02 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"
#include <signal.h>

// Initialize the pipex structure
	// Atempt to open the input file from argv[1] for reading
	// If open fails, print error message then open /dev/null to continue pipe
	// Store outfile path, cmd1, cmd2, and environment variables
	// Create array of two file descriptors for the pipe: Read and Write ends
void	init_pipex(t_pipex *pipex, char **argv, char **envp)
{
	int	empty_pipe[2];

	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		{
			if (pipe(empty_pipe) == -1)
				ft_pipex_error("pipex: pipe create empty pipe", 1);
			close(empty_pipe[1]);
			pipex->infile = empty_pipe[0];
		}
	}
	pipex->outfile_path = argv[4];
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
	pipex->envp = envp;
	if (pipe(pipex->pipe_fds) == -1)
		ft_pipex_error("pipex: pipe creation failed", 1);
}

// First child process
	// Close the read end of the pipe, not used by this child process
	// Redirect the standard input to the input file
	// Redirect the standard output to the write end of the pipe
	// Close the original descriptors after redirection
	// Execute the first command
static void	first_child(t_pipex *pipex)
{
	close(pipex->pipe_fds[0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		ft_pipex_error("pipex: first_child dup2 infile failed", 1);
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
		ft_pipex_error("pipex: first_child dup2 pipe_fds[1] failed", 1);
	close(pipex->infile);
	close(pipex->pipe_fds[1]);
	exec_cmd(pipex->cmd1, pipex->envp);
}

// Second child process
	// Close the write end of the pipe
	// Redirect the standard input to the read end of the pipe
	// Open the output file for writing	
	// Redirect the standard output to the output file
	// Execute the second command
static void	second_child(t_pipex *pipex)
{
	close(pipex->pipe_fds[1]);
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
		ft_pipex_error("pipex: second_child dup2 pipe_fds[0] failed", 1);
	pipex->outfile = open(pipex->outfile_path,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		ft_pipex_error("pipex: second_child open outfile failed", 1);
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		ft_pipex_error("pipex: second_child dup2 outfile failed", 1);
	close(pipex->pipe_fds[0]);
	close(pipex->outfile);
	exec_cmd(pipex->cmd2, pipex->envp);
}

// Execute the pipex command
	// Fork the first child process (to exec cmd1)
	// Child: Run the first child process
	// Parent: Close the input file descriptor (not needed anymore)
	// Fork the second child process (to exec cmd2)
	// Child: Run the second child process
	// Parent: Close the read and write end of the pipe
	// Parent Wait for the first and second child processes to finish
	// Check and return the exit status of the second child process
int	exec_pipex(t_pipex *pipex)
{
	int	status1;
	int	status2;

	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		ft_pipex_error("pipex: fork failed", 1);
	if (pipex->pid1 == 0)
		first_child(pipex);
	close(pipex->infile);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		ft_pipex_error("pipex: fork failed", 1);
	if (pipex->pid2 == 0)
		second_child(pipex);
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	waitpid(pipex->pid1, &status1, 0);
	waitpid(pipex->pid2, &status2, 0);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	return (0);
}
