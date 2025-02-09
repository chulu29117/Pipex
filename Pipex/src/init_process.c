/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:34:59 by clu               #+#    #+#             */
/*   Updated: 2025/02/07 14:58:13 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Initialize the pipex structure
// Open the input file
// If the input file cannot be opened, open /dev/null
// If /dev/null cannot be opened, exit with an error message
// Open the output file
// If the output file cannot be opened, exit with an error message
// Set the command strings
// Set the environment variables
// Create a pipe
void	init_pipex(t_pipex *pipex, char **argv, char **envp)
{
	pipex->infile = open(argv[1], O_RDONLY);
	if (pipex->infile < 0)
	{
		perror("pipex: input");
		pipex->infile = open("/dev/null", O_RDONLY);
		if (pipex->infile < 0)
			pipex_error("pipex: failed to open /dev/null", 1);
	}
	pipex->outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex->outfile < 0)
		pipex_error("pipex: output", 1);
	pipex->cmd1 = argv[2];
	pipex->cmd2 = argv[3];
	pipex->envp = envp;
	if (pipe(pipex->pipe_fds) == -1)
		pipex_error("pipex: pipe creation failed", 1);
}
