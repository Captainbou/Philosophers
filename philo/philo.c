/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:58:41 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/23 17:47:12 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int c, char **v)
{
	t_pdata	*pdata;
	t_philo	*philos;
	int		i;

	if (init_data(&philos, &pdata, c, v))
		return (1);
	if (init_philos_data(pdata, philos))
		return (1);
	i = 0;
	while (i < pdata->noph)
	{
		if (pthread_create((&philos[i].thread), NULL, philo, &philos[i]) != 0)
			return (ft_destroy(pdata, "Error: Thread creation failed\n"));
		i++;
	}
	if (monitor(philos))
		return (1);
	i = 0;
	while (i < pdata->noph)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	ft_destroy(pdata, NULL);
}
