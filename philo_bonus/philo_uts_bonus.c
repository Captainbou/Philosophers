/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_uts_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 22:53:56 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/03 16:24:02 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	started(t_pdata *pdata)
{
	sem_wait(pdata->sem_start);
	sem_post(pdata->sem_start);
	return (1);
}

int	init_philo(t_pdata *pdata, t_philo *philos, int i)
{
	while (!started(pdata))
		;
	if (pthread_create(&philos[i].thread, NULL, monitor, &philos[i]))
	{
		ft_destroy(pdata, NULL);
		sem_post(pdata->sem_death);
		exit(1);
	}
	philo(&philos[i]);
	pthread_join(philos[i].thread, NULL);
	ft_destroy(pdata, NULL);
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
			ft_destroy(pdata, "Error: Fork failed\n");
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
