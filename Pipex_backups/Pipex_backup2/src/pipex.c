/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 18:29:28 by clu               #+#    #+#             */
/*   Updated: 2025/02/23 22:37:27 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

// Check number of arguments
	// Initialize the pipex structure
	// Execute the pipex command
int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;
	int		exit_code;

	if (argc != 5)
	{
		ft_putstr_fd("Usage: ./pipex file1 cmd1 cmd2 file2\n", STDERR_FILENO);
		exit(1);
	}
	init_pipex(&pipex, argv, envp);
	exit_code = exec_pipex(&pipex);
	return (exit_code);
}
