/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 16:06:39 by clu               #+#    #+#             */
/*   Updated: 2025/01/30 22:25:11 by clu              ###   ########.fr       */
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
typedef struct s_pipex
{
	int		pipe_fds[2];
	int		infile;
	int		outfile;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

void	ft_error(const char *message);
void	sys_error(const char *message);

void	exec_cmd(char *cmd, char **envp);
void	first_child(t_pipex *pipex, char *cmd, char **envp);
void	second_child(t_pipex *pipex, char *cmd, char **envp);
void	exec_pipe(t_pipex *pipex);

char	**split_cmd(char *cmd);
char	*find_path(char *cmd, char **envp);

void	init_pipex(t_pipex *pipex, char **argv);
void	process(t_pipex *pipex, char **argv, char **envp);

#endif
