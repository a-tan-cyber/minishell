/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_cnt.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 10:23:06 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 12:02:59 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_count_cins(const char *str, char c)
{
	int	count;

	if (!str)
		return (0);
	count = 0;
	while (*str)
	{
		if (*str == c)
			count += 1;
		str++;
	}
	return (count);
}
