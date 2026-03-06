/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_assign_cd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 17:19:43 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 17:19:48 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static const char	*ms_assign_cd_pick(t_var *vars, char **argv, int *print_pwd)
{
	const char	*target;

	*print_pwd = 0;
	if (!argv[1])
	{
		target = ms_var_get(vars, "HOME");
		if (!target)
			ft_putendl_fd("moonshell: cd: HOME not set", STDERR_FILENO);
		return (target);
	}
	if (!ft_strcmp(argv[1], "-"))
	{
		target = ms_var_get(vars, "OLDPWD");
		if (!target)
			ft_putendl_fd("moonshell: cd: OLDPWD not set", STDERR_FILENO);
		*print_pwd = 1;
		return (target);
	}
	return (argv[1]);
}

static int	ms_assign_cd_fail(const char *target, char **oldpwd)
{
	ft_putstr_fd("moonshell: cd: ", STDERR_FILENO);
	ft_putstr_fd((char *)target, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
	ft_sfree((void **)oldpwd);
	return (1);
}

static int	ms_assign_cd_apply(t_info *i, const char *target, int print_pwd)
{
	char	*oldpwd;
	char	*newpwd;

	oldpwd = getcwd(NULL, 0);
	if (chdir(target) != 0)
		return (ms_assign_cd_fail(target, &oldpwd));
	newpwd = getcwd(NULL, 0);
	if (print_pwd)
	{
		if (newpwd)
			ft_putendl_fd(newpwd, STDOUT_FILENO);
		else
			ft_putendl_fd((char *)target, STDOUT_FILENO);
	}
	if (oldpwd)
		ms_var_set(&i->vars, "OLDPWD", oldpwd, TRUE);
	if (newpwd)
		ms_var_set(&i->vars, "PWD", newpwd, TRUE);
	ms_var_sync_env(i);
	ft_sfree((void **)&oldpwd);
	ft_sfree((void **)&newpwd);
	return (0);
}

int	ms_try_assign_cd(t_info *i, t_ast *cmd)
{
	t_var		*tmp;
	const char	*pick;
	char		*target;
	int			n;
	int			print_pwd;

	if (!i || !cmd || !cmd->args || !cmd->args[0])
		return (0);
	n = ms_assign_prefix_len(cmd->args);
	if (n == 0 || !cmd->args[n] || ft_strcmp(cmd->args[n], "cd"))
		return (0);
	tmp = ms_var_dup(i->vars);
	if (!tmp && i->vars)
		return (i->err = 1, 1);
	if (ms_assign_apply_n(&tmp, cmd->args, n, FALSE))
		return (ms_var_clear(&tmp), i->err = 1, 1);
	pick = ms_assign_cd_pick(tmp, cmd->args + n, &print_pwd);
	if (!pick)
		return (ms_var_clear(&tmp), i->err = 1, 1);
	target = ft_strdup(pick);
	ms_var_clear(&tmp);
	if (!target)
		return (i->err = 1, 1);
	i->err = ms_assign_cd_apply(i, target, print_pwd);
	return (ft_sfree((void **)&target), 1);
}
