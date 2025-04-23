/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:43:46 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/23 17:52:02 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	ft_usleep(long time, t_pdata *pdata)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
	{
		if (check_death(pdata))
			return ;
		usleep(100);
	}
}

int	eating(t_philo *philos, t_pdata *pdata)
{
	pthread_mutex_lock((philos->right_fork));
	print_message(philos, "has taken a fork");
	if (philos->right_fork != philos->left_fork)
		pthread_mutex_lock((philos->left_fork));
	else
		return (1);
	print_message(philos, "has taken a fork");
	pthread_mutex_lock(&philos->meal_mutex);
	philos->last_meal_time = get_time();
	philos->number_of_times_eaten++;
	if (philos->number_of_times_eaten == philos->pdata->number_of_times_to_eat)
		philos->is_full = 1;
	pthread_mutex_unlock(&philos->meal_mutex);
	print_message(philos, "is eating");
	ft_usleep(pdata->time_to_eat, philos->pdata);
	pthread_mutex_unlock((philos->right_fork));
	pthread_mutex_unlock((philos->left_fork));
	return (0);
}

void	*philo(void *param)
{
	t_philo	*philos;
	t_pdata	*pdata;

	philos = (t_philo *)param;
	pdata = philos->pdata;
	if (philos->id % 2 == 0)
	{
		usleep(1000);
	}
	while (1)
	{
		pthread_mutex_lock(&philos->pdata->is_full_mutex);
		if (philos->pdata->is_all_full == 1)
			return (pthread_mutex_unlock(&philos->pdata->is_full_mutex), NULL);
		pthread_mutex_unlock(&philos->pdata->is_full_mutex);
		if (check_death(philos->pdata))
			return (NULL);
		if (eating(philos, pdata))
			return (NULL);
		print_message(philos, "is sleeping");
		ft_usleep(pdata->time_to_sleep, philos->pdata);
		print_message(philos, "is thinking");
	}
	return (NULL);
}
