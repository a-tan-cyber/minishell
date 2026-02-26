/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:58:07 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 19:36:28 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	init_ms_var(t_ast **astree, t_info *i)
{
	*astree = NULL;
	i->err = 0;
	i->line = NULL;
	i->my_env = NULL;
	i->lexed = NULL;
	i->ast = *astree;
}
