/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:45:20 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/23 17:45:39 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_counted(int i, int *counted)
{
	if (counted[i] == 1)
		return (1);
	counted[i] = 1;
	return (0);
}

int	check_end_of_service(int i, int j, t_philo *philos)
{
	if (j == philos->pdata->noph)
	{
		pthread_mutex_lock(&philos[i].pdata->is_full_mutex);
		philos->pdata->is_all_full = 1;
		pthread_mutex_unlock(&philos[i].pdata->is_full_mutex);
		return (1);
	}
	if (get_time() - philos[i].last_meal_time >= philos->pdata->time_to_die)
	{
		print_message(&philos[i], "died");
		pthread_mutex_lock(&philos[i].pdata->death_mutex);
		philos[i].pdata->is_dead = 1;
		pthread_mutex_unlock(&philos[i].pdata->death_mutex);
		pthread_mutex_lock(&philos->pdata->print_mutex);
		pthread_mutex_unlock(&philos->pdata->print_mutex);
		return (1);
	}
	return (0);
}

int	*init_counted(int *j, int size)
{
	int	*counted;

	*j = 0;
	counted = ft_malloc(sizeof(int) * size, GB);
	if (!counted)
		return (NULL);
	memset(counted, 0, sizeof(int) * size);
	return (counted);
}

int	monitor(t_philo *philos)
{
	int	i;
	int	j;
	int	*counted;

	counted = init_counted(&j, philos->pdata->noph);
	if (!counted)
		return (ft_destroy(philos->pdata, "Error: Memory allocation failed\n"));
	while (1)
	{
		i = 0;
		while (i < philos->pdata->noph)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			if (philos[i].is_full == 1 && !is_counted(i, counted))
				j++;
			if (check_end_of_service(i, j, philos))
				return (pthread_mutex_unlock(&philos[i].meal_mutex), 0);
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (0);
}
