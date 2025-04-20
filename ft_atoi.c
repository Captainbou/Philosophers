
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

int	ft_atoi(char *str, int *err)
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
	if (res >= 2147483647 || res <= -2147483648)
		return (-1);
	return (res * sign);
}
