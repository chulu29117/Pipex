/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/01/28 13:18:31 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	if (argc != 5)
		return (ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n"), 1);
	pipex.infile = open(argv[1], O_RDONLY);
	if (pipex.infile == -1)
		sys_error("Error: Unable to open input file");
	pipex.outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pipex.outfile == -1)
		sys_error("Error: Unable to open/create output file");
	if (pipe(pipex.pipe_fds) == -1)
		sys_error("Error: Pipe failed");
	pipex.pid1 = fork();
	if (pipex.pid1 == -1)
		sys_error("Error: Fork failed");
	if (pipex.pid1 == 0)
		first_child(&pipex, argv[2], envp);
	pipex.pid2 = fork();
	if (pipex.pid2 == -1)
		sys_error("Error: Fork failed");
	if (pipex.pid2 == 0)
		second_child(&pipex, argv[3], envp);
	close_pipes_and_wait(&pipex);
	return (0);
}
