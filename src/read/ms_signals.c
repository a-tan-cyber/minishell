/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_signals.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/21 05:56:54 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/21 05:56:54 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	int_handler(int sig)
{
	g_sig = sig;
}

char	*read_multiline_sigint(char *rslt)
{
	g_sig = 0;
	ft_sfree(&rslt);
	write(1, "\n", 1);
	return (ft_strdup(""));
}
