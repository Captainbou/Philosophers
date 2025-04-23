/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:32:50 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/23 17:33:45 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_data2(t_philo **philos, t_pdata **pdata)
{
	int	i;

	*philos = ft_malloc(sizeof(t_philo) * (*pdata)->noph, GB);
	if (!*philos)
		return (ft_destroy(*pdata, "Error: Memory allocation failed\n"));
	(*pdata)->forks = ft_malloc(sizeof(pthread_mutex_t) * (*pdata)->noph, GB);
	if (!(*pdata)->forks)
		return (ft_destroy(*pdata, "Error: Memory allocation failed\n"));
	i = 0;
	while (i < (*pdata)->noph)
	{
		if (pthread_mutex_init((&(*pdata)->forks[i]), NULL) != 0)
			return (ft_destroy(*pdata, "Error: Mutex initialization failed\n"));
		i++;
	}
	(*pdata)->start_time = get_time();
	if (pthread_mutex_init(&(*pdata)->print_mutex, NULL) != 0
		|| pthread_mutex_init(&(*pdata)->death_mutex, NULL) != 0
		|| pthread_mutex_init(&(*pdata)->is_full_mutex, NULL) != 0)
		return (ft_destroy(*pdata, "Error: Mutex initialization failed\n"));
	return (0);
}

int	handle_args(t_pdata **pdata, int c, char **v)
{
	int	err;

	err = 0;
	(*pdata)->noph = ft_atol(v[1], &err);
	if ((*pdata)->noph <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid number of philos\n"));
	(*pdata)->time_to_die = ft_atol(v[2], &err);
	if ((*pdata)->time_to_die <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid time to die\n"));
	(*pdata)->time_to_eat = ft_atol(v[3], &err);
	if ((*pdata)->time_to_eat <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid time to eat\n"));
	(*pdata)->time_to_sleep = ft_atol(v[4], &err);
	if ((*pdata)->time_to_sleep <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid time to sleep\n"));
	if (c == 6)
	{
		(*pdata)->number_of_times_to_eat = ft_atol(v[5], &err);
		if ((*pdata)->number_of_times_to_eat <= 0 || err)
			return (ft_destroy(*pdata, "Err: Invalid num of times to eat\n"));
	}
	else
		(*pdata)->number_of_times_to_eat = -1;
	return (0);
}

int	init_data(t_philo **philos, t_pdata **pdata, int c, char **v)
{
	if (c != 5 && c != 6)
		return (write(2, "Error: Invalid number of arguments\n", 36), 1);
	*pdata = ft_malloc(sizeof(t_pdata), GB);
	if (!*pdata)
		return (write(2, "Error: Memory allocation failed\n", 33), 1);
	memset(*pdata, 0, sizeof(t_pdata));
	if (handle_args(pdata, c, v))
		return (1);
	return (init_data2(philos, pdata));
}

int	init_philos_data(t_pdata *pdata, t_philo *philos)
{
	int	i;

	i = 0;
	memset(philos, 0, sizeof(t_philo) * pdata->noph);
	while (i < pdata->noph)
	{
		philos[i].right_fork = &pdata->forks[i];
		philos[i].left_fork = &pdata->forks[(i + 1) % pdata->noph];
		philos[i].id = i + 1;
		philos[i].last_meal_time = get_time();
		if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
			return (ft_destroy(pdata, "Error: Mutex initialization failed\n"));
		philos[i].pdata = pdata;
		i++;
	}
	return (0);
}
