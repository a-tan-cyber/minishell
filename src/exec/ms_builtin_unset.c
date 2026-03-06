/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ms_builtin_unset.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:17:53 by amtan             #+#    #+#             */
/*   Updated: 2026/03/06 15:12:32 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ms_builtin_unset(t_info *i, char **argv)
{
	int	j;

	if (!i || !argv)
		return (1);
	j = 1;
	while (argv[j])
	{
		ms_var_unset(&i->vars, argv[j]);
		j++;
	}
	if (ms_var_sync_env(i))
		return (1);
	return (0);
}
