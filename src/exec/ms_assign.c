/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_assign.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:52:59 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 16:55:49 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_assign_ident(const char *arg, size_t *eq)
{
	size_t	i;

	if (!arg || !arg[0] || !(ft_isalpha(arg[0]) || arg[0] == '_'))
		return (0);
	i = 1;
	while (arg[i] && arg[i] != '=')
	{
		if (!(ft_isalnum(arg[i]) || arg[i] == '_'))
			return (0);
		i++;
	}
	if (arg[i] != '=')
		return (0);
	if (eq)
		*eq = i;
	return (1);
}

static int	ms_assign_set_one(t_var **vars, const char *arg, t_bool exported)
{
	size_t	eq;
	char	*name;
	int		ret;

	if (!ms_assign_ident(arg, &eq))
		return (1);
	name = ft_substr(arg, 0, eq);
	if (!name)
		return (1);
	ret = ms_var_set(vars, name, arg + eq + 1, exported);
	ft_sfree((void **)&name);
	return (ret);
}

int	ms_assign_apply_n(t_var **vars, char **argv, int n, t_bool exported)
{
	int	j;

	j = 0;
	while (j < n)
	{
		if (ms_assign_set_one(vars, argv[j], exported))
			return (1);
		j++;
	}
	return (0);
}

int	ms_assign_prefix_len(char **argv)
{
	int	j;

	j = 0;
	while (argv && argv[j] && ms_assign_ident(argv[j], NULL))
		j++;
	return (j);
}

int	ms_try_assign_only(t_info *i, t_ast *cmd)
{
	int	n;

	if (!i || !cmd || !cmd->args || !cmd->args[0] || cmd->rdir)
		return (0);
	n = ms_assign_prefix_len(cmd->args);
	if (n == 0 || cmd->args[n])
		return (0);
	i->err = ms_assign_apply_n(&i->vars, cmd->args, n, FALSE);
	if (!i->err)
		i->err = ms_var_sync_env(i);
	return (1);
}
