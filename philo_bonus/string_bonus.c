/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:18:00 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/12 20:18:49 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_strncmp(const char *s1, const char *s2, int n)
{
	unsigned char	*t1;
	unsigned char	*t2;
	int				i;

	i = 0;
	t1 = (unsigned char *)s1;
	t2 = (unsigned char *)s2;
	if (n == 0)
		return (0);
	while (t1[i] && t2[i] && t1[i] == t2[i] && i < n)
		i++;
	if (i < n)
		return (t1[i] - t2[i]);
	return (0);
}

int	ft_strlen(const char *s)
{
	const char	*current = s;

	if (!s)
		return (0);
	while (*current)
		current++;
	return (current - s);
}

char	*ft_strjoin(char const *s1, char *s2)
{
	char	*str;
	int		j;
	int		i;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (free(s2),NULL);
	str = (char *)malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!str)
		return (free(s2),NULL);
	while (s1[i])
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j])
	{
		str[i + j] = s2[j];
		j++;
	}
	free(s2);
	str[i + j] = '\0';
	return (str);
}

int	int_lenght(int num)
{
	int	i;

	i = 0;
	if (num < 0)
		i++;
	while (num / 10)
	{
		i++;
		num = num / 10;
	}
	return (i + 1);
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	num;

	num = n;
	len = int_lenght(n);
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n < 0)
		num *= -1;
	while (len)
	{
		str[--len] = (num % 10) + '0';
		num = num / 10;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
