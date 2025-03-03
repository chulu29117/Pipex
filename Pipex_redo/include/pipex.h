/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:06:39 by clu               #+#    #+#             */
/*   Updated: 2025/03/02 13:38:36 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <string.h>
# include <errno.h>
# include "libft.h"

// Structure for pipex
typedef struct s_pipex
{
	int		pipe_fds[2];
	int		infile;
	int		outfile;
	char	*cmd_path;
	char	**envp_paths;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

// Execute the pipex command
void	exec_cmd(char *cmd, char **envp, t_pipex *pipex);

// Find the full path of the command
char	*prep_cmd_path(char *cmd, char **envp, t_pipex *pipex);
char	*find_path(char *cmd, char **envp);

// Error handling
void	exit_error(const char *msg);
void	put_error(const char *msg);
void	cmd_error(const char *cmd);

#endif
