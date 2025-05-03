/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:43:46 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/02 21:44:41 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	ft_usleep(long time)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
		usleep(500);
}

void	eating(t_philo *philos, t_pdata *pdata)
{
	sem_wait(philos->pdata->forks);
	print_message(philos, "has taken a fork");
	sem_wait(philos->pdata->forks);
	print_message(philos, "has taken a fork");
	philos->last_meal_time = get_time();
	philos->number_of_times_eaten++;
	if (philos->number_of_times_eaten == philos->pdata->number_of_times_to_eat)
		philos->is_full = 1;
	print_message(philos, "is eating");
	ft_usleep(pdata->time_to_eat);
	sem_post(philos->pdata->forks);
	sem_post(philos->pdata->forks);
}

void	*philo(t_philo *philos)
{
	t_pdata	*pdata;

	pdata = philos->pdata;
	if (philos->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (philos->is_full == 1)
			return (NULL);
		eating(philos, pdata);
		print_message(philos, "is sleeping");
		ft_usleep(pdata->time_to_sleep);
		print_message(philos, "is thinking");
	}
	return (NULL);
}
