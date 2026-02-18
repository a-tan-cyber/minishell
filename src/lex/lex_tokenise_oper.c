/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokenise_oper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:49:26 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 17:49:26 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	tokenise_oper_or(t_token **lexed)
{
	if (token_push_back(lexed, OR, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_pipe(t_token **lexed)
{
	if (token_push_back(lexed, PIPE, NULL) == NULL)
		return (0);
	return (1);
}
size_t	tokenise_oper_and(t_token **lexed)
{
	if (token_push_back(lexed, AND, NULL) == NULL)
		return (0);
	return (2);
}
