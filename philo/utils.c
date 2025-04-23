/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:46:50 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/23 17:47:41 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_destroy(t_pdata *pdata, char *err_msg)
{
	int	i;

	i = 0;
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
	while (i < pdata->noph)
	{
		if (pdata->forks)
			pthread_mutex_destroy(&pdata->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&pdata->print_mutex);
	pthread_mutex_destroy(&pdata->death_mutex);
	pthread_mutex_destroy(&pdata->is_full_mutex);
	ft_malloc(0, GB_CLEAR);
	return (1);
}

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		;
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}

int	check_death(t_pdata *data)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->is_dead)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}

void	print_message(t_philo *philos, char *message)
{
	static int	j = 0;

	if (!check_death(philos->pdata))
	{
		pthread_mutex_lock(&philos->pdata->print_mutex);
		if (j)
		{
			pthread_mutex_unlock(&philos->pdata->print_mutex);
			return ;
		}
		if (!ft_strncmp(message, "died", 4))
			j = 1;
		printf("%ld %d %s\n", get_time() - philos->pdata->start_time,
			philos->id, message);
		pthread_mutex_unlock(&philos->pdata->print_mutex);
	}
}
