/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 21:35:05 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/02 21:35:08 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

int	ft_skip(char *str, int *i, int *sign)
{
	while ((str[*i] >= 9 && str[*i] <= 13) || str[*i] == 32)
		*i += 1;
	if ((str[*i] == '-' || str[*i] == '+'))
	{
		if (str[*i] == '-')
			*sign = -1;
		*i += 1;
		if (!str[*i])
			return (0);
	}
	return (1);
}

int	get_number(char *str, int i, int sign, long *res)
{
	long	r1;

	while (str[i])
	{
		if (ft_isdigit(str[i]))
		{
			r1 = *res;
			*res = *res * 10 + str[i] - '0';
			if (*res / 10 != r1 && sign == 1)
				return (-1);
			if (*res / 10 != r1 && sign == -1)
				return (-1);
			i++;
		}
		else
			return (0);
	}
	return (1);
}

long	ft_atol(char *str, int *err)
{
	int		sign;
	int		i;
	long	res;

	sign = 1;
	res = 0;
	i = 0;
	ft_skip(str, &i, &sign);
	if (get_number(str, i, sign, &res) <= 0)
	{
		if (err)
			*err = 1;
		return (0);
	}
	return (res * sign);
}
