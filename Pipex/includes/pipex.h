/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:06:39 by clu               #+#    #+#             */
/*   Updated: 2025/02/04 10:25:24 by clu              ###   ########.fr       */
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

// Struct holding pipex data
typedef struct	s_pipex
{
	int		pipe_fds[2];
	int		infile;
	int		outfile;
	char	*cmd1;
	char	*cmd2;
	char	**cmd_paths;
	char	**envp;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

void	init_pipex(t_pipex *pipex, char **argv, char **envp);
void	exec_pipex(t_pipex *pipex);

void	first_child(t_pipex *pipex);
void	second_child(t_pipex *pipex);
void	exec_cmd(char *cmd, char **envp);

void	pipex_error(const char *msg);
char	**split_cmd(char *cmd);
char	*find_path(char *cmd, char **envp);

#endif
