/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 23:43:19 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/12 20:12:54 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_destroy(t_pdata *pdata, t_philo *philos, char *err_msg)
{
	int	i;

	i = 0;
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
	sem_close(pdata->print_sem);
	sem_close(pdata->forks);
	sem_close(pdata->sem_death);
	sem_close(pdata->servant);
	while (i < pdata->noph)
	{
		free(philos[i].sem_name);
		sem_close(philos[i++].last_meal);
	}
	free(philos->pdata);
	free(philos);
	return (1);
}

long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		;
	return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
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
	printf("%ld %d %s\n", get_time() - philos->pdata->start_time, philos->id,
		message);
	sem_post(philos->pdata->print_sem);
}
