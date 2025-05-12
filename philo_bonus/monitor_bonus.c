/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:45:20 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/12 15:19:29 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	check_end_of_service(t_philo *philos)
{
	if (!philos->is_eating && get_time() - philos->last_meal_time >= philos->pdata->time_to_die)
	{
		print_message(philos, "died");
		sem_post(philos->last_meal);
		return (1);
	}
	sem_post(philos->last_meal);
	return (0);
}

void	*monitor(void *param)
{
	t_philo	*philos;

	philos = (t_philo *)param;
	while (1)
	{
		sem_wait(philos->last_meal);
		if (philos->is_full == 1)
			return (sem_post(philos->last_meal), NULL);
		if (check_end_of_service(philos))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}
