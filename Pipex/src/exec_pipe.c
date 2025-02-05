/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:07:41 by clu               #+#    #+#             */
/*   Updated: 2025/02/05 11:56:09 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child(t_pipex *pipex)
{
	close(pipex->pipe_fds[0]);
	if (dup2(pipex->infile, STDIN_FILENO) == -1)
		pipex_error("pipex: first_child dup2 infile failed", 1);
	if (dup2(pipex->pipe_fds[1], STDOUT_FILENO) == -1)
		pipex_error("pipex: first_child dup2 pipe_fds[1] failed", 1);
	close(pipex->infile);
	close(pipex->pipe_fds[1]);
	exec_cmd(pipex->cmd1, pipex->envp);
	pipex_error("pipex: first_child exec_cmd failed", 1);
}

void	second_child(t_pipex *pipex)
{
	close(pipex->pipe_fds[1]);
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
		pipex_error("pipex: second_child dup2 pipe_fds[0] failed", 1);
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
		pipex_error("pipex: second_child dup2 outfile failed", 1);
	close(pipex->pipe_fds[0]);
	close(pipex->outfile);
	exec_cmd(pipex->cmd2, pipex->envp);
	pipex_error("pipex: second_child exec_cmd failed", 1);
}

static void	fork_child_1(t_pipex *pipex)
{
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		pipex_error("pipex: fork failed", 1);
	if (pipex->pid1 == 0)
		first_child(pipex);
}

int	exec_pipex(t_pipex *pipex)
{
	int	status1;
	int	status2;

	fork_child_1(pipex);
	close(pipex->infile);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		pipex_error("pipex: fork failed", 1);
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
