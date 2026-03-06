/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_assign_builtin.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:06:09 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 17:06:12 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_assign_builtin_id(const char *s)
{
	if (!ft_strcmp(s, "echo"))
		return (1);
	if (!ft_strcmp(s, "pwd"))
		return (2);
	if (!ft_strcmp(s, "env"))
		return (3);
	return (0);
}

static int	ms_assign_builtin_temp(t_info *tmp, t_info *src, char **argv, int n)
{
	*tmp = *src;
	tmp->vars = ms_var_dup(src->vars);
	if (!tmp->vars && src->vars)
		return (1);
	tmp->my_env = NULL;
	if (ms_assign_apply_n(&tmp->vars, argv, n, TRUE))
		return (ms_var_clear(&tmp->vars), 1);
	if (ms_var_sync_env(tmp))
		return (ms_var_clear(&tmp->vars), 1);
	return (0);
}

static void	ms_assign_builtin_free(t_info *tmp)
{
	if (!tmp)
		return ;
	free_arr(tmp->my_env);
	tmp->my_env = NULL;
	ms_var_clear(&tmp->vars);
}

static void	ms_assign_builtin_run(t_info *tmp, t_ast *cmd, int id)
{
	int	save_in;
	int	save_out;

	if (ms_redir_parent_begin(cmd->rdir, &save_in, &save_out))
	{
		tmp->err = 1;
		return ;
	}
	if (id == 1)
		tmp->err = ms_builtin_echo(cmd->args);
	else if (id == 2)
		tmp->err = ms_builtin_pwd();
	else
		tmp->err = ms_builtin_env(tmp);
	ms_redir_parent_end(save_in, save_out);
}

int	ms_try_assign_builtin(t_info *i, t_ast *cmd)
{
	t_info	tmp;
	t_ast	fake;
	int		n;
	int		id;

	if (!i || !cmd || !cmd->args || !cmd->args[0])
		return (0);
	n = ms_assign_prefix_len(cmd->args);
	if (n == 0 || !cmd->args[n])
		return (0);
	id = ms_assign_builtin_id(cmd->args[n]);
	if (!id)
		return (0);
	if (ms_assign_builtin_temp(&tmp, i, cmd->args, n))
		return (i->err = 1, 1);
	fake = *cmd;
	fake.args = cmd->args + n;
	ms_assign_builtin_run(&tmp, &fake, id);
	i->err = tmp.err;
	ms_assign_builtin_free(&tmp);
	return (1);
}
