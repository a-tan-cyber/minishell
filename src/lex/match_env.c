/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   match_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amtan <amtan@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 17:52:55 by yunguo            #+#    #+#             */
/*   Updated: 2026/02/26 19:28:44 by amtan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// return index if present, -1 if not present
int	ft_envcmp(char *str, const char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		if (ft_strncmp(str, arr[i], ft_strlen(str)) == 0
			&& arr[i][ft_strlen(str)] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*ft_extract_env(int i, const char **arr)
{
	int	j;

	j = 0;
	while (arr[i][j] && arr[i][j] != '=')
		j++;
	return (ft_strdup(arr[i] + j + 1));
}

// ?; number
char	*match_env(char *snippet, const char **env, t_info *info)
{
	if (ft_strcmp(snippet, "?") == 0)
		return (ft_itoa(info->err));
	else if (ft_str_is_num(snippet) == 1)
		return (ft_strdup(""));
	else if (ft_envcmp(snippet, env) != -1)
		return (ft_extract_env(ft_envcmp(snippet, env), env));
	return (ft_strdup(""));
}
