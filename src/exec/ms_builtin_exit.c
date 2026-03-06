/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_exit.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 01:24:04 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 23:14:13 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ms_exit_print(t_info *i)
{
	if (i && i->interactive)
		write(STDOUT_FILENO, "exit\n", 5);
}

static int	ms_exit_init(t_exitnum *n, const char *s)
{
	n->s = s;
	n->i = 0;
	n->sign = 1;
	if (s[n->i] == '+' || s[n->i] == '-')
	{
		if (s[n->i] == '-')
			n->sign = -1;
		n->i++;
	}
	if (!ft_isdigit(s[n->i]))
		return (1);
	n->lim = (unsigned long long)LLONG_MAX;
	if (n->sign == -1)
		n->lim++;
	n->acc = 0;
	return (0);
}

static int	ms_exit_accum(t_exitnum *n)
{
	unsigned int	d;

	while (ft_isdigit(n->s[n->i]))
	{
		d = (unsigned int)(n->s[n->i] - '0');
		if (n->acc > (n->lim - d) / 10)
			return (1);
		n->acc = n->acc * 10 + d;
		n->i++;
	}
	if (n->s[n->i] != '\0')
		return (1);
	return (0);
}

static int	ms_exit_parse_ll(const char *s, long long *out)
{
	t_exitnum	n;

	if (!s || !out)
		return (1);
	if (ms_exit_init(&n, s))
		return (1);
	if (ms_exit_accum(&n))
		return (1);
	if (n.sign == -1)
	{
		if (n.acc == n.lim)
			*out = LLONG_MIN;
		else
			*out = -(long long)n.acc;
	}
	else
		*out = (long long)n.acc;
	return (0);
}

int	ms_builtin_exit(t_info *i, char **argv)
{
	long long	n;

	if (!i || !argv || !argv[0])
		return (1);
	ms_exit_print(i);
	if (argv[1] && argv[2])
		return (ft_putendl_fd("moonshell: exit: too many arguments",
				STDERR_FILENO), i->err = 1, 0);
	if (!argv[1])
		return (i->exit_req = TRUE, 0);
	if (ms_exit_parse_ll(argv[1], &n))
	{
		ft_putstr_fd("moonshell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putendl_fd(": numeric argument required", STDERR_FILENO);
		return (i->err = 2, i->exit_req = TRUE, 0);
	}
	i->err = (unsigned char)n;
	i->exit_req = TRUE;
	return (0);
}
