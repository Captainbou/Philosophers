/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:43:19 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/26 15:07:48 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_destroy(t_pdata *pdata, char *err_msg)
{
	int	i;

	i = 0;
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
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

int	check_death(t_philo *data)
{
	if (data->is_dead)
	{
		return (1);
	}
	return (0);
}

void	print_message(t_philo *philos, char *message)
{

	sem_wait(philos->pdata->print_sem);
	if (!ft_strncmp(message, "died", 4))
	{
		printf("%ld %d %s\n", get_time() - philos->pdata->start_time,
			philos->id, message);
		sem_post(philos->pdata->sem_death);
		return ;
	}
	printf("%ld %d %s\n", get_time() - philos->pdata->start_time,
			philos->id, message);
	sem_post(philos->pdata->print_sem);
}
