/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: clu <clu@student.hive.fi>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 21:42:32 by clu               #+#    #+#             */
/*   Updated: 2025/02/03 13:44:13 by clu              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	sys_error(const char *message)
{
	ft_putstr_fd("pipex: ", 2);
	if (message && *message)
	{
		ft_putstr_fd(message, 2);
		if (errno)
		{
			ft_putstr_fd(": ", 2);
			ft_putstr_fd(strerror(errno), 2);
		}
	}
	ft_putstr_fd("\n", 2);
}
