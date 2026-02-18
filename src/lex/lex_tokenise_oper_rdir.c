/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lex_tokenise_oper_rdir.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:55:30 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/18 17:55:30 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	tokenise_oper_heredoc(t_token **lexed)
{
	if (token_push_back(lexed, HEREDOC, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_rediin(t_token **lexed)
{
	if (token_push_back(lexed, REDI_IN, NULL) == NULL)
		return (0);
	return (1);
}
size_t	tokenise_oper_append(t_token **lexed)
{
	if (token_push_back(lexed, APPEND, NULL) == NULL)
		return (0);
	return (2);
}
size_t	tokenise_oper_rediot(t_token **lexed)
{
	if (token_push_back(lexed, REDI_OT, NULL) == NULL)
		return (0);
	return (1);
}
