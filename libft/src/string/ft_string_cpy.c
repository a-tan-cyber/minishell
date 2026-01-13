/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_cpy.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 11:42:20 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/libft.h"

char	*ft_strcpy(char *dest, char *src)
{
	int	count;

	count = 0;
	while (src[count] != '\0')
	{
		dest[count] = src[count];
		count++;
	}
	dest[count] = '\0';
	return (dest);
}

char	*ft_strncpy(char *dest, char *src, unsigned int n)
{
	unsigned int	count;

	count = 0;
	while (count < n && src[count] != '\0')
	{
		dest[count] = src[count];
		count++;
	}
	while (count < n)
	{
		dest[count] = '\0';
		count++;
	}
	return (dest);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	unsigned int	count;
	unsigned int	src_len;

	src_len = 0;
	while (src[src_len])
	{
		src_len++;
	}
	if (size > 0)
	{
		count = 0;
		while (count < size - 1 && src[count])
		{
			dest[count] = src[count];
			count++;
		}
		dest[count] = '\0';
	}
	return (src_len);
}
