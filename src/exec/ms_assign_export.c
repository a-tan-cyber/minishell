/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_assign_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:59:11 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 18:50:48 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ms_assign_export_match(t_ast *cmd, int *n)
{
	if (!cmd || !cmd->args || !cmd->args[0] || !n)
		return (0);
	*n = ms_assign_prefix_len(cmd->args);
	if (*n == 0 || !cmd->args[*n])
		return (0);
	return (!ft_strcmp(cmd->args[*n], "export"));
}

static int	ms_assign_export_ref(const char *arg, const char *assign)
{
	size_t	eq;

	if (!arg || !assign || !ms_assign_ident(assign, &eq))
		return (0);
	if (ft_strchr(arg, '='))
		return (0);
	if (ft_strncmp(arg, assign, eq))
		return (0);
	return (arg[eq] == '\0');
}

static int	ms_assign_export_seed(t_info *i, char **argv, int n, char **expv)
{
	int		j;
	int		k;
	size_t	eq;
	char	*name;

	j = 0;
	while (j < n)
	{
		k = 1;
		while (expv[k] && !ms_assign_export_ref(expv[k], argv[j]))
			k++;
		if (expv[k] && ms_assign_ident(argv[j], &eq))
		{
			name = ft_strndup(argv[j], eq);
			if (!name)
				return (1);
			if (ms_var_set(&i->vars, name, argv[j] + eq + 1, FALSE))
				return (ft_sfree((void **)&name), 1);
			ft_sfree((void **)&name);
		}
		j++;
	}
	return (0);
}

static void	ms_assign_export_run(t_info *i, t_ast *cmd, int n)
{
	t_ast	fake;
	int		save_in;
	int		save_out;

	fake = *cmd;
	fake.args = cmd->args + n;
	if (ms_redir_parent_begin(cmd->rdir, &save_in, &save_out))
	{
		i->err = 1;
		return ;
	}
	i->err = ms_builtin_export(i, fake.args);
	ms_redir_parent_end(save_in, save_out);
}

int	ms_try_assign_export(t_info *i, t_ast *cmd)
{
	int	n;

	if (!i || !cmd)
		return (0);
	if (!ms_assign_export_match(cmd, &n))
		return (0);
	if (ms_assign_export_seed(i, cmd->args, n, cmd->args + n))
		return (i->err = 1, 1);
	ms_assign_export_run(i, cmd, n);
	return (1);
}
