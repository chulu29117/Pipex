/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:07:41 by clu               #+#    #+#             */
/*   Updated: 2025/02/24 10:41:41 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Initialize the pipex structure
void	init_pipex(t_pipex *pipex, char **argv, char **envp)
{
	// Open the input file
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		// If open fails, print error message then open /dev/null to continue the pipeline
		ft_putstr_fd("pipex: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
		perror("");
		pipex->infile = open("/dev/null", O_RDONLY);
		if (pipex->infile < 0)
			ft_pipex_error("pipex: failed to open /dev/null", 1);
	}
	// Store the output file path, the first command, the second command, and the environment variables
	pipex->outfile_path = argv[4];
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
	pipex->envp = envp;
	// Create file descriptors for the pipe (pipe_fds[0] for reading, pipe_fds[1] for writing)
	if (pipe(pipex->pipe_fds) == -1)
		ft_pipex_error("pipex: pipe creation failed", 1);
}

// First child process
static void	first_child(t_pipex *pipex)
{
	// Close the read end of the pipe, not used by this child process
	close(pipex->pipe_fds[0]);
	// Redirect the standard input to the input file
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		ft_pipex_error("pipex: first_child dup2 infile failed", 1);
	// Redirect the standard output to the write end of the pipe
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
		ft_pipex_error("pipex: first_child dup2 outfile failed", 1);
	// Close the original descriptors after redirection
	close(pipex->infile);
	close(pipex->pipe_fds[1]);
	// Execute the first command
	exec_cmd(pipex->cmd1, pipex->envp);
}

// Second child process
static void	second_child(t_pipex *pipex)
{
	// Close the write end of the pipe
	close(pipex->pipe_fds[1]);
	// Redirect the standard input to the read end of the pipe
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
		ft_pipex_error("pipex: second_child dup2 pipe_fds[0] failed", 1);
	// Open the output file for writing	
	pipex->outfile = open(pipex->outfile_path,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
		ft_pipex_error("pipex: second_child open outfile failed", 1);
	// Redirect the standard output to the output file
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		ft_pipex_error("pipex: second_child dup2 outfile failed", 1);
	// Close the read end of the pipe and the output file descriptor
	close(pipex->pipe_fds[0]);
	close(pipex->outfile);
	// Execute the second command
	exec_cmd(pipex->cmd2, pipex->envp);
}

// Execute the pipex command
int	exec_pipex(t_pipex *pipex)
{
	int	status1;
	int	status2;

	// Fork the first child process (to exec cmd1)
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		ft_pipex_error("pipex: fork failed", 1);
	// Child: Run the first child process
	if (pipex->pid1 == 0)
		first_child(pipex);
	// Parent: Close the input file descriptor (not needed anymore)
	close(pipex->infile);
	// Fork the second child process (to exec cmd2)
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		ft_pipex_error("pipex: fork failed", 1);
	// Child: Run the second child process
	if (pipex->pid2 == 0)
		second_child(pipex);
	// Parent: Close the read and write end of the pipe
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	// Parent Wait for the first and second child processes to finish
	waitpid(pipex->pid1, &status1, 0);
	waitpid(pipex->pid2, &status2, 0);
	// Check and return the exit status of the second child process
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	if (WIFEXITED(status1))
		return (WEXITSTATUS(status1));
	return (1);
}
