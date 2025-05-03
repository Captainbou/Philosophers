/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:58:41 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/02 22:53:12 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*ripping(void *param)
{
	t_philo	*philos;
	int		i;

	i = 0;
	philos = (t_philo *)param;
	sem_wait(philos->pdata->sem_death);
	while (i < philos->pdata->noph)
	{
		kill(philos[i].pid, SIGKILL);
		i++;
	}
	return (NULL);
}

void	kill_orphans(t_philo *philos, int i)
{
	int	j;

	j = 0;
	while (j < i)
	{
		if (philos[j].pid > 0)
			kill(philos[j].pid, SIGKILL);
		waitpid(philos[j].pid, NULL, 0);
		j++;
	}
}

int	init_sems(t_pdata *pdata)
{
	sem_unlink("/forks");
	sem_unlink("/print_sem");
	sem_unlink("/sem_death");
	sem_unlink("/sem_start");
	pdata->forks = sem_open("/forks", O_CREAT, 0644, pdata->noph);
	pdata->print_sem = sem_open("/print_sem", O_CREAT, 0644, 1);
	pdata->sem_death = sem_open("/sem_death", O_CREAT, 0644, 0);
	pdata->sem_start = sem_open("/sem_start", O_CREAT, 0644, 0);
	if (!pdata->forks || !pdata->print_sem || !pdata->sem_death
		|| !pdata->sem_start)
		return (1);
	sem_unlink("/forks");
	sem_unlink("/print_sem");
	sem_unlink("/sem_death");
	sem_unlink("/sem_start");
	return (0);
}

int	main(int c, char **v)
{
	t_pdata		*pdata;
	t_philo		*philos;
	pthread_t	ripper;

	if (init_data(&philos, &pdata, c, v))
		return (1);
	if (init_philos_data(pdata, philos))
		return (1);
	if (init_sems(pdata))
		return (ft_destroy(pdata, "sem_open failed\n"));
	if (philo_birth(pdata, philos))
		return (1);
	sem_post(pdata->sem_start);
	if (pthread_create(&ripper, NULL, ripping, philos))
	{
		kill_orphans(philos, pdata->noph);
		ft_destroy(pdata, "Error: thread creation failed\n");
		return (1);
	}
	while (waitpid(-1, NULL, 0) > 0)
		;
	sem_post(pdata->sem_death);
	pthread_join(ripper, NULL);
	ft_destroy(pdata, NULL);
}
