/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:59 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 11:26:58 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
		perror("pipex: infile error");
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
		pipex_error("pipex: outfile error");
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
	pipex->envp = envp;
	if (pipe(pipex->pipe_fds) == -1)
		pipex_error("pipex: pipe creation failed");
}

void	process(t_pipex *pipex)
{
	int status;

	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
		pipex_error("pipex: fork failed");
	if (pipex->pid1 == 0)
		first_child(pipex);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
		pipex_error("pipex: fork failed");
	if (pipex->pid2 == 0)
		second_child(pipex);
	close(pipex->pipe_fds[0]);
	close(pipex->pipe_fds[1]);
	waitpid(pipex->pid1, NULL, 0);
	waitpid(pipex->pid2, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	exit(EXIT_FAILURE);
}

