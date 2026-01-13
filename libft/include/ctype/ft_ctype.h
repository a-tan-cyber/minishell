/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ctype.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yunguo <yunguo@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 07:32:52 by yunguo            #+#    #+#             */
/*   Updated: 2025/12/09 12:05:51 by yunguo           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_CTYPE_H
# define FT_CTYPE_H

// ctype.c
int				ft_isalpha(int c);
int				ft_isdigit(int c);
int				ft_isalnum(int c);
int				ft_isascii(int c);
int				ft_is_white_space(char base);
// int	ft_isprint(int c);

//strtobit.c
char			*ft_conv_chartobitstr(unsigned char x, char *byte);
char			*ft_conv_strtobitstr(char *msg, char *bitstr);
unsigned char	ft_conv_bitstrtochar(char *byte);

#endif