/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 23:07:41 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 11:26:25 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	first_child(t_pipex *pipex)
{
	dup2(pipex->infile, STDIN_FILENO);
	dup2(pipex->pipe_fds[1], STDOUT_FILENO);
	close(pipex->pipe_fds[0]);
	close(pipex->infile);
	exec_cmd(pipex->cmd1, pipex->envp);
}

void	second_child(t_pipex *pipex)
{
	if (dup2(pipex->pipe_fds[0], STDIN_FILENO) == -1)
	{
		perror("DEBUG: dup2 input failed");
		exit(1);
	}
	if (dup2(pipex->outfile, STDOUT_FILENO) == -1)
	{
		perror("DEBUG: dup2 output failed");
		exit(1);
	}
	close(pipex->pipe_fds[1]);
	close(pipex->outfile);
	exec_cmd(pipex->cmd2, pipex->envp);
	exit(1);
}

void	exec_pipex(t_pipex *pipex)
{
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
		first_child(pipex);
	pipex->pid2 = fork();
	if (pipex->pid2 == 0)
		second_child(pipex);
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	waitpid(pipex->pid1, NULL, 0);
	waitpid(pipex->pid2, NULL, 0);
}
