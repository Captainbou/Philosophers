/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 17:43:46 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/12 15:08:20 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_usleep(long time, t_pdata *pdata)
{
	long	start_time;

	start_time = get_time();
	while (get_time() - start_time < time)
	{
		if (check_death(pdata))
			return (1);
		usleep(500);
	}
	return (0);
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
	if (check_death(philos->pdata))
		return (pthread_mutex_unlock((philos->right_fork)),
			pthread_mutex_unlock((philos->left_fork)), 1);
	pthread_mutex_lock(&philos->meal_mutex);
	philos->is_eating = 1;
	philos->last_meal_time = get_time();
	philos->number_of_times_eaten++;
	if (philos->number_of_times_eaten == philos->pdata->number_of_times_to_eat)
		philos->is_full = 1;
	pthread_mutex_unlock(&philos->meal_mutex);
	print_message(philos, "is eating");
	if (ft_usleep(pdata->time_to_eat, philos->pdata))
		return (pthread_mutex_unlock((philos->right_fork)),
			pthread_mutex_unlock((philos->left_fork)), 1);
	pthread_mutex_unlock((philos->right_fork));
	pthread_mutex_unlock((philos->left_fork));
	pthread_mutex_lock(&philos->meal_mutex);
	philos->is_eating = 0;
	pthread_mutex_unlock(&philos->meal_mutex);
	return (0);
}

int	started(t_philo *philo)
{
	pthread_mutex_lock(&philo->pdata->start);
	pthread_mutex_unlock(&philo->pdata->start);
	return (1);
}

void	*philo(void *param)
{
	t_philo	*philos;
	t_pdata	*pdata;

	philos = (t_philo *)param;
	pdata = philos->pdata;
	while (!started(philos))
		;
	if (philos->id % 2 == 0)
		usleep(1000);
	while (1)
	{
		if (check_death(philos->pdata))
			return (NULL);
		pthread_mutex_lock(&philos->pdata->is_full_mutex);
		if (philos->pdata->is_all_full == 1)
			return (pthread_mutex_unlock(&philos->pdata->is_full_mutex), NULL);
		pthread_mutex_unlock(&philos->pdata->is_full_mutex);
		if (eating(philos, pdata))
			return (NULL);
		print_message(philos, "is sleeping");
		ft_usleep(pdata->time_to_sleep, philos->pdata);
		print_message(philos, "is thinking");
	}
	return (NULL);
}
