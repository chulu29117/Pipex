/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/02/20 12:58:11 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		exit_code;

	// Check number of arguments
	if (argc != 5)
	{
		// errno = EINVAL;	// EINVAL: Invalid argument
		ft_pipex_error("Usage: ./pipex file1 cmd1 cmd2 file2", 1);
	}
	// Initialize the pipex structure with command-line arguments and environment
	init_pipex(&pipex, argv, envp);
	// Execute the pipeline and collect its exit status
	exit_code = exec_pipex(&pipex);
	return (exit_code);
}
