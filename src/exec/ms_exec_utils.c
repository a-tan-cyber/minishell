/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_exec_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 23:39:35 by amtan             #+#    #+#             */
/*   Updated: 2026/03/05 23:44:02 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ms_exec_child_fail(const char *cmd)
{
	struct stat	st;

	ft_putstr_fd("moonshell: ", STDERR_FILENO);
	ft_putstr_fd((char *)cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (stat(cmd, &st) == 0 && S_ISDIR(st.st_mode))
		ft_putendl_fd("Is a directory", STDERR_FILENO);
	else
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
	if (errno == ENOENT)
		exit(127);
	exit(126);
}
