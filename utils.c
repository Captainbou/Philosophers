int	ft_strncmp(const char *s1, const char *s2, int n)
{
	unsigned char	*t1;
	unsigned char	*t2;
	int			i;

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