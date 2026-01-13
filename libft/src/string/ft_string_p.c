/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_string_p.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 12:02:38 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>

void	ft_putchar(char c)
{
	write(1, &c, 1);
}

void	ft_putstr(const char *str)
{
	while (*str)
	{
		write(1, &(*str), 1);
		str++;
	}
}

void	ft_puterr(char *str)
{
	while (*str)
	{
		write(2, &(*str), 1);
		str++;
	}
}

void	ft_putnstr(char *str, int n)
{
	write(1, &(*str), n);
}

void	ft_putnbr(int nb)
{
	int		negative;
	char	digit;

	negative = 1;
	if (nb < 0)
	{
		write(1, "-", 1);
		negative = -1;
	}
	if (-10 < nb && nb < 10)
	{
		digit = nb * negative;
		digit = digit + '0';
	}
	else
	{
		digit = nb % 10;
		digit = digit * negative;
		digit = digit + '0';
		ft_putnbr((int)(negative * (nb / 10)));
	}
	write(1, &digit, 1);
}
