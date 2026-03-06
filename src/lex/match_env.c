/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:52:55 by yunguo            #+#    #+#             */
/*   Updated: 2026/03/06 15:39:04 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*match_env(char *snippet, t_info *info)
{
	char	*value;

	if (ft_strcmp(snippet, "?") == 0)
		return (ft_itoa(info->err));
	if (ft_str_is_num(snippet) == 1)
		return (ft_strdup(""));
	value = ms_var_get(info->vars, snippet);
	if (value)
		return (ft_strdup(value));
	return (ft_strdup(""));
}
