/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42singapore.sg>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:58:07 by yunguo            #+#    #+#             */
/*   Updated: 2026/01/13 10:05:30 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	init_ms_var(t_ast **astree, t_info *i)
{
	*astree = NULL;
	i->err = NULL;
	i->line = NULL;
	i->my_env = NULL;
	i->lexed = NULL;
	i->ast = *astree;
}

