/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_uts_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 22:53:56 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/12 18:07:17 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	init_philo(t_pdata *pdata, t_philo *philos, int i)
{
	if (pthread_create(&philos[i].thread, NULL, monitor, &philos[i]))
	{
		ft_destroy(pdata, philos, "Error: thread creation failed");
		sem_close(philos->last_meal);
		sem_post(pdata->sem_death);
		exit(1);
	}
	philo(&philos[i]);
	pthread_join(philos[i].thread, NULL);
	sem_close(philos->last_meal);
	ft_destroy(pdata, philos, NULL);
	exit(0);
}

int	philo_birth(t_pdata *pdata, t_philo *philos)
{
	int	i;

	i = 0;
	while (i < pdata->noph)
	{
		philos[i].pid = fork();
		if (philos[i].pid < 0)
		{
			kill_orphans(philos, i);
			ft_destroy(pdata, philos, "Error: Fork failed\n");
			return (1);
		}
		if (philos[i].pid == 0)
		{
			init_philo(pdata, philos, i);
		}
		i++;
	}
	return (0);
}
