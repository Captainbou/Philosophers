/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:18:00 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/23 23:18:34 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	const char *current = s;

	if (!s)
		return (0);
	while (*current)
		current++;
	return (current - s);
}