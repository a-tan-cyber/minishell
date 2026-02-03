/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctype_special.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/25 21:09:00 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/25 21:09:00 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

t_bool	is_operator(char c)
{
	if (c == '|' || c == '<' || c == '>' || c == '&')
		return (TRUE);
	return (FALSE);
}

t_bool	is_bracket(char c)
{
	if (c == '(' || c == ')')
		return (TRUE);
	return (FALSE);
}