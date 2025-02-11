/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:07:41 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 18:01:03 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// First child process
// Close the read end of the pipe
// Redirect the standard input to the input file
// Redirect the standard output to the write end of the pipe
// Close the input file descriptor and the write end of the pipe
// Execute the first command
void	first_child(t_pipex *pipex)
{
	close(pipex->pipe_fds[0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		ft_pipex_error("pipex: first_child dup2 infile failed", 1);
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
		ft_pipex_error("pipex: first_child dup2 outfile failed", 1);
	close(pipex->infile);
	close(pipex->pipe_fds[1]);
	exec_cmd(pipex->cmd1, pipex->envp);
	ft_pipex_error("pipex: first_child exec_cmd failed", 1);
}

// Second child process
// Close the write end of the pipe
// Redirect the standard input to the read end of the pipe
// Redirect the standard output to the output file
// Close the read end of the pipe and the output file descriptor
// Execute the second command
void	second_child(t_pipex *pipex)
{
	close(pipex->pipe_fds[1]);
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
		ft_pipex_error("pipex: second_child dup2 pipe_fds[0] failed", 1);
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		ft_pipex_error("pipex: second_child dup2 outfile failed", 1);
	close(pipex->pipe_fds[0]);
	close(pipex->outfile);
	exec_cmd(pipex->cmd2, pipex->envp);
	ft_pipex_error("pipex: second_child exec_cmd failed", 1);
}

// Execute the pipex command
// Fork the first child process
// Close the input file descriptor
// Fork the second child process
// Close the read and write end of the pipe
// Wait for the second child and first child processes to finish
// Close the output file descriptor
// Return the exit status of the second child process
// If the second child process does not exit, exit first child process
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
	waitpid(pipex->pid2, &status2, 0);
	waitpid(pipex->pid1, &status1, 0);
	close(pipex->outfile);
	if (WIFEXITED(status2))
		return (WEXITSTATUS(status2));
	if (WIFEXITED(status1))
		return (WEXITSTATUS(status1));
	return (1);
}
