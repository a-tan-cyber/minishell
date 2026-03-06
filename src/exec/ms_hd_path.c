/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_hd_path.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 11:47:11 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 11:47:14 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ms_hd_path(int idx)
{
	char	*num;
	char	*path;

	num = ft_itoa(idx);
	if (!num)
		return (NULL);
	path = ft_strjoin2("/tmp/moonshell_hd_", num);
	ft_sfree((void **)&num);
	return (path);
}
