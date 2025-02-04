/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:59 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 23:21:46 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		perror("pipex: line 1: input");
		pipex->infile = open("/dev/null", O_RDONLY);
		if (pipex->infile < 0)
			pipex_error("pipex: failed to open /dev/null");
	}
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
		pipex_error("pipex: outfile error");
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
	pipex->envp = envp;
	if (pipe(pipex->pipe_fds) == -1)
		pipex_error("pipex: pipe creation failed");
}

// void	init_pipex(t_pipex *pipex, char **argv, char **envp)
// {
// 	pipex->infile = open(argv[1], O_RDONLY);
// 	if (pipex->infile < 0)
// 		perror("pipex: infile error");
// 	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (pipex->outfile < 0)
// 		pipex_error("pipex: outfile error");
// 	pipex->cmd1 = argv[2];
// 	pipex->cmd2 = argv[3];
// 	pipex->envp = envp;
// 	if (pipe(pipex->pipe_fds) == -1)
// 		pipex_error("pipex: pipe creation failed");
// }

