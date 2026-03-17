/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_free.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 09:57:27 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/17 22:44:38 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_token_one(t_token **lst)
{
	t_token	*node;
	t_token	*next;

	if (!lst || !*lst)
		return ;
	node = *lst;
	next = node->next;
	if (node->prev)
		node->prev->next = node->next;
	if (node->next)
		node->next->prev = node->prev;
	*lst = next;
	ft_sfree((void **)&node->text);
	ft_sfree((void **)&node->raw);
	ft_sfree((void **)&node);
}

void	free_token_lst(t_token **lst)
{
	while (lst && *lst)
		free_token_one(lst);
}

static void	free_null_var(t_ast **ast, char ***env, char **line,
							t_token **lexed)
{
	if (ast != NULL)
	{
		free_ast(*ast);
		*ast = NULL;
	}
	if (env != NULL)
	{
		free_arr(*env);
		*env = NULL;
	}
	if (line != NULL)
	{
		ft_sfree((void **)line);
		*line = NULL;
	}
	if (lexed != NULL)
	{
		free_token_lst(lexed);
		*lexed = NULL;
	}
}

void	free_ms_var(t_ast **astree, t_info *i, char *cmd)
{
	if (ft_strcmp(cmd, "tmp") == 0)
		free_null_var(astree, NULL, &i->line, &i->lexed);
	else if (ft_strcmp(cmd, "all") == 0)
	{
		free_null_var(astree, &i->my_env, &i->line, &i->lexed);
		ft_sfree((void **)&i->input_buf);
		ms_var_clear(&i->vars);
		ms_history_clear(i);
	}
	i->ast = NULL;
}

void	ms_exit_child(t_info *i, int code)
{
	free_ms_var(&i->ast, i, "all");
	exit(code);
}
