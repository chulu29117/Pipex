/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/11 16:59:38 by clu               #+#    #+#             */
/*   Updated: 2025/02/11 17:22:07 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	write_here_doc(int fd, char *limiter)
{
	char	*line;
	size_t	len;

	line = get_next_line(0);
	len = ft_strlen(limiter);
	while (line)
	{
		if (ft_strncmp(line, limiter, len) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
		line = get_next_line(0);
	}
}

int	here_doc(char *limiter)
{
	int		temp_fd;

	temp_fd = open("tempfile", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (temp_fd < 0)
		pipex_error("Error opening temp file", 1);
	write_here_doc(temp_fd, limiter);
	close(temp_fd);
	temp_fd = open("here_doc_tempfile", O_RDONLY);
	if (temp_fd < 0)
		pipex_error("Error opening temp file", 1);
	return (temp_fd);
}

