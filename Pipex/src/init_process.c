/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:59 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 10:21:14 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY);
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
	pipex->envp = envp;
	if (pipex->infile < 0 || pipex->outfile < 0)
		pipex_error("File error");
	if (pipe(pipex->pipe_fds) == -1)
		pipex_error("Pipe failed");
}

void process(t_pipex *pipex, char **argv, char **envp)
{
    int status;

    pipex->pid1 = fork();
    if (pipex->pid1 == -1)
        pipex_error("pipex: fork failed", EXIT_FAILURE);
    if (pipex->pid1 == 0)
        first_child(pipex, argv[2], envp);
    pipex->pid2 = fork();
    if (pipex->pid2 == -1)
        pipex_error("pipex: fork failed", EXIT_FAILURE);
    if (pipex->pid2 == 0)
        second_child(pipex, argv[3], envp);
    close(pipex->pipe_fds[0]);
    close(pipex->pipe_fds[1]);
    waitpid(pipex->pid2, &status, 0);
    if (WIFEXITED(status))
        exit(WEXITSTATUS(status));
    exit(EXIT_FAILURE);
}
