/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_init.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:58:07 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/08 12:24:26 by amtan            ###   ########.fr       */
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
	i->input_buf = NULL;
	i->line_no = 1;
	i->cmd_line_no = 1;
	i->my_env = NULL;
	i->vars = NULL;
	i->lexed = NULL;
	i->ast = *astree;
	i->exit_req = FALSE;
}
