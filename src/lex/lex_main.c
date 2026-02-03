/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/24 18:40:38 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/24 18:40:38 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	skip_whitespace(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] && ft_is_white_space(line[i]))
	{
		i++;
	}
	return (i);
}
