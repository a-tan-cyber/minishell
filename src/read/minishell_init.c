/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:58:07 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 01:23:41 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_ms_var(t_ast **astree, t_info *i)
{
	*astree = NULL;
	i->err = 0;
	i->interactive = FALSE;
	i->hist = NULL;
	i->line = NULL;
	i->my_env = NULL;
	i->lexed = NULL;
	i->ast = *astree;
	i->exit_req = FALSE;
}
