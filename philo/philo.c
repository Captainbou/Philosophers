/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:58:41 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/22 20:40:36 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_destroy(t_pdata *pdata, char *err_msg)
{
	int	i;

	i = 0;
	if (err_msg)
		write(2, err_msg, ft_strlen(err_msg));
	while (i < pdata->noph)
	{
		if (pdata->forks)
			pthread_mutex_destroy(&pdata->forks[i]);
		i++;
	}
	pthread_mutex_destroy(&pdata->print_mutex);
	pthread_mutex_destroy(&pdata->death_mutex);
	pthread_mutex_destroy(&pdata->is_full_mutex);
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
int	check_death(t_pdata *data)
{
	pthread_mutex_lock(&data->death_mutex);
	if (data->is_dead)
	{
		pthread_mutex_unlock(&data->death_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->death_mutex);
	return (0);
}
void	print_message(t_philo *philos, char *message)
{
	static int	j = 0;

	if (!check_death(philos->pdata))
	{
		pthread_mutex_lock(&philos->pdata->print_mutex);
		if (j)
		{
			pthread_mutex_unlock(&philos->pdata->print_mutex);
			return ;
		}
		if (!ft_strncmp(message, "died", 4))
			j = 1;
		printf("%ld %d %s\n", get_time() - philos->pdata->start_time,
			philos->id, message);
		pthread_mutex_unlock(&philos->pdata->print_mutex);
	}
}

int	is_counted(int i, int *counted)
{
	if (counted[i] == 1)
		return (1);
	counted[i] = 1;
	return (0);
}
int	check_end_of_service(int i, int j, t_philo *philos)
{
	if (j == philos->pdata->noph)
	{
		pthread_mutex_lock(&philos[i].pdata->is_full_mutex);
		philos->pdata->is_all_full = 1;
		pthread_mutex_unlock(&philos[i].pdata->is_full_mutex);
		return (1);
	}
	if (get_time() - philos[i].last_meal_time > philos->pdata->time_to_die)
	{
		print_message(&philos[i], "died");
		pthread_mutex_lock(&philos[i].pdata->death_mutex);
		philos[i].pdata->is_dead = 1;
		pthread_mutex_unlock(&philos[i].pdata->death_mutex);
		pthread_mutex_lock(&philos->pdata->print_mutex);
		pthread_mutex_unlock(&philos->pdata->print_mutex);
		return (1);
	}
	return (0);
}

int	monitor(t_philo *philos)
{
	int	i;
	int	j;
	int *counted;

	j = 0;
	counted = malloc(sizeof(int) * philos->pdata->noph);
	if (!counted)
		return (1);
	memset(counted, 0, sizeof(int) * philos->pdata->noph);
	while (1)
	{
		i = 0;
		while (i < philos->pdata->noph)
		{
			pthread_mutex_lock(&philos[i].meal_mutex);
			if (philos[i].is_full == 1 && !is_counted(i, counted))
				j++;
			if (check_end_of_service(i, j, philos))
			{
				pthread_mutex_unlock(&philos[i].meal_mutex);
				return (0);
			}
			pthread_mutex_unlock(&philos[i].meal_mutex);
			i++;
		}
		usleep(1000);
	}
	return (0);
}

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
int	init_data2(t_philo **philos, t_pdata **pdata)
{
	int	i;

	*philos = ft_malloc(sizeof(t_philo) * (*pdata)->noph, GB);
	if (!*philos)
		return (ft_destroy(*pdata, "Error: Memory allocation failed\n"));
	(*pdata)->forks = ft_malloc(sizeof(pthread_mutex_t) * (*pdata)->noph, GB);
	if (!(*pdata)->forks)
		return (ft_destroy(*pdata, "Error: Memory allocation failed\n"));
	i = 0;
	while (i < (*pdata)->noph)
	{
		if (pthread_mutex_init((&(*pdata)->forks[i]), NULL) != 0)
			return (ft_destroy(*pdata, "Error: Mutex initialization failed\n"));
		i++;
	}
	(*pdata)->start_time = get_time();
	if (pthread_mutex_init(&(*pdata)->print_mutex, NULL) != 0
		|| pthread_mutex_init(&(*pdata)->death_mutex, NULL) != 0
		|| pthread_mutex_init(&(*pdata)->is_full_mutex, NULL) != 0)
		return (ft_destroy(*pdata, "Error: Mutex initialization failed\n"));
	return (0);
}

int	handle_args(t_pdata **pdata, int c, char **v)
{
	int	err;

	err = 0;
	(*pdata)->noph = ft_atol(v[1], &err);
	if ((*pdata)->noph <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid number of philos\n"));
	(*pdata)->time_to_die = ft_atol(v[2], &err);
	if ((*pdata)->time_to_die <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid time to die\n"));
	(*pdata)->time_to_eat = ft_atol(v[3], &err);
	if ((*pdata)->time_to_eat <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid time to eat\n"));
	(*pdata)->time_to_sleep = ft_atol(v[4], &err);
	if ((*pdata)->time_to_sleep <= 0 || err)
		return (ft_destroy(*pdata, "Error: Invalid time to sleep\n"));
	if (c == 6)
	{
		(*pdata)->number_of_times_to_eat = ft_atol(v[5], &err);
		if ((*pdata)->number_of_times_to_eat <= 0 || err)
			return (ft_destroy(*pdata, "Err: Invalid num of times to eat\n"));
	}
	else
		(*pdata)->number_of_times_to_eat = -1;
	return (0);
}

int	init_data(t_philo **philos, t_pdata **pdata, int c, char **v)
{
	if (c != 5 && c != 6)
		return (write(2, "Error: Invalid number of arguments\n", 36), 1);
	*pdata = ft_malloc(sizeof(t_pdata), GB);
	if (!*pdata)
		return (write(2, "Error: Memory allocation failed\n", 33), 1);
	memset(*pdata, 0, sizeof(t_pdata));
	if (handle_args(pdata, c, v))
		return (1);
	return (init_data2(philos, pdata));
}

int	init_philos_data(t_pdata *pdata, t_philo *philos)
{
	int	i;

	i = 0;
	memset(philos, 0, sizeof(t_philo) * pdata->noph);
	while (i < pdata->noph)
	{
		philos[i].right_fork = &pdata->forks[i];
		philos[i].left_fork = &pdata->forks[(i + 1) % pdata->noph];
		philos[i].id = i + 1;
		philos[i].last_meal_time = get_time();
		if (pthread_mutex_init(&philos[i].meal_mutex, NULL) != 0)
			return (ft_destroy(pdata, "Error: Mutex initialization failed\n"));
		philos[i].pdata = pdata;
		i++;
	}
	return (0);
}

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
	monitor(philos);
	i = 0;
	while (i < pdata->noph)
	{
		pthread_join(philos[i].thread, NULL);
		i++;
	}
	ft_destroy(pdata, NULL);
}
