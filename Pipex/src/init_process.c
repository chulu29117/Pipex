/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:59 by clu               #+#    #+#             */
/*   Updated: 2025/02/03 13:46:33 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	init_pipex(t_pipex *pipex, char **argv)
{
	if (argv[2][0] == '\0' || argv[3][0] == '\0')
		sys_error("invalid empty command");
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile == -1)
	{
		sys_error(argv[1]);
		pipex->infile = open("/dev/null", O_RDONLY);
		if (pipex->infile == -1)
		{
			sys_error("/dev/null");
			exit(1);
		}
	}
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile == -1)
	{
		sys_error(argv[4]);
		pipex->outfile = -1;
	}
	if (pipe(pipex->pipe_fds) == -1)
	{
		sys_error("Pipe failed");
		exit(1);
	}
}

void	process(t_pipex *pipex, char **argv, char **envp)
{
	pipex->pid1 = fork();
	if (pipex->pid1 == -1)
	{
		sys_error("Fork failed");
		exit(1);
	}
	if (pipex->pid1 == 0)
		first_child(pipex, argv[2], envp);
	pipex->pid2 = fork();
	if (pipex->pid2 == -1)
	{
		sys_error("Fork failed");
		exit(1);
	}
	if (pipex->pid2 == 0)
		second_child(pipex, argv[3], envp);
}
