/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 15:58:41 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/21 18:16:05 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time()
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        ;
    return ((tv.tv_sec * 1000 + tv.tv_usec / 1000));
}
int check_death(t_pdata *data)
{
    pthread_mutex_lock(&data->death_mutex);
    if(data->is_dead)
    {
        pthread_mutex_unlock(&data->death_mutex);
        return(1);
    }
    pthread_mutex_unlock(&data->death_mutex);
    return(0);
}
void print_message(t_philo *philos, char *message)
{
    static int j = 0;
    if(!check_death(philos->pdata))
    {
        
        pthread_mutex_lock(&philos->pdata->print_mutex);
        if(j)
        {
            pthread_mutex_unlock(&philos->pdata->print_mutex);
            return;
        }
        if(!ft_strncmp(message, "died", 4))
            j = 1;
        printf("%ld %d %s\n", get_time() - philos->pdata->start_time, philos->id, message);
        pthread_mutex_unlock(&philos->pdata->print_mutex);
    }
}

int is_counted(int i)
{
    static int counted[200];

    if (counted[i] == 1)
        return (1);
    counted[i] = 1;
    return (0);
}
void monitor(t_philo *philos)
{
    long time;
    int i;
    int j;
    int z = 0;

    j = 0;
    while (1)
    {
        i = 0;
        while (i < philos->pdata->noph)
        {
            pthread_mutex_lock(&philos[i].meal_mutex);
            if ( philos[i].is_full == 1 && !is_counted(i))
                j++;
            if (j == philos->pdata->noph)
            {
                pthread_mutex_lock(&philos[i].pdata->is_full_mutex);
                philos->pdata->is_all_full = 1;
                pthread_mutex_unlock(&philos[i].meal_mutex);
                pthread_mutex_unlock(&philos[i].pdata->is_full_mutex);
                return;
            }
            if (get_time() - philos[i].last_meal_time > philos->pdata->time_to_die)
            {   
                pthread_mutex_unlock(&philos[i].meal_mutex);
                print_message(&philos[i], "died");
                pthread_mutex_lock(&philos[i].pdata->death_mutex);
                philos[i].pdata->is_dead = 1;
                pthread_mutex_unlock(&philos[i].pdata->death_mutex);
                pthread_mutex_lock(&philos->pdata->print_mutex);
                pthread_mutex_unlock(&philos->pdata->print_mutex);
                return;
            }
            pthread_mutex_unlock(&philos[i].meal_mutex);
            i++;
        }
        usleep(2000);
    }
}

void ft_usleep(long time, t_pdata *pdata)
{
    long start_time;
    start_time = get_time();
    
    while (get_time() - start_time < time)
    {
        if (check_death(pdata))
            return;
        usleep(100);
    }
}

void *philo(void *param)
{
    t_philo *philos = (t_philo *)param;
    t_pdata *pdata = philos->pdata;
    int i = 0;

    if (philos->id % 2 == 0)
    {
        usleep(1000);
    }
    while (1)
    {
        pthread_mutex_lock(&philos->pdata->is_full_mutex);
        if(philos->pdata->is_all_full == 1)
            return (pthread_mutex_unlock(&philos->pdata->is_full_mutex),NULL);
        pthread_mutex_unlock(&philos->pdata->is_full_mutex);
        if(check_death(philos->pdata))
            return(NULL);
        pthread_mutex_lock((philos->right_fork));
        print_message(philos, "has taken a fork");
        if (philos->right_fork != philos->left_fork)
            pthread_mutex_lock((philos->left_fork));
        else
            return (NULL);
        print_message(philos, "has taken a fork");
        pthread_mutex_lock(&philos->meal_mutex);
        philos->last_meal_time = get_time();
        philos->number_of_times_eaten++;
        if(philos->number_of_times_eaten == philos->pdata->number_of_times_to_eat)
            philos->is_full = 1;
        pthread_mutex_unlock(&philos->meal_mutex);
        print_message(philos, "is eating");
        ft_usleep(pdata->time_to_eat, philos->pdata);
        pthread_mutex_unlock((philos->right_fork));
        pthread_mutex_unlock((philos->left_fork));
        print_message(philos, "is sleeping");
        ft_usleep(pdata->time_to_sleep, philos->pdata);
        print_message(philos, "is thinking");
    }
    return (NULL);
}

int main(int c, char **v)
{
    t_pdata *pdata;
    t_philo *philos;
    struct timeval tv;
    int i;
    int err;
    
    pdata = ft_malloc(sizeof(t_pdata),GB);
    if(!pdata)
    {
        write(2, "Error: Memory allocation failed\n", 32);
        return (1);
    }
    i = 0;
    if (c != 5 && c != 6)
    {
        write(2,"Error: Invalid number of arguments\n",36);
        return (1);
    }
    pdata->noph = ft_atol(v[1],&err);
    if (pdata->noph <= 0 || err)
    {
        write(2, "Error: Invalid number of philosophers\n", 39);
        ft_malloc(0,GB_CLEAR);
        return (1);
    }
    philos = ft_malloc(sizeof(t_philo) * pdata->noph,GB);
    if(!philos)
    {
        write(2, "Error: Memory allocation failed\n", 32);
        ft_malloc(0,GB_CLEAR);
        return (1);
    }
    pdata->forks = ft_malloc(sizeof(pthread_mutex_t) * pdata->noph,GB);
    if(!pdata->forks)
    {
        write(2, "Error: Memory allocation failed\n", 32);
        ft_malloc(0,GB_CLEAR);
        return (1);
    }
    while(i < pdata->noph)
    {
        pthread_mutex_init((&pdata->forks[i]), NULL);
        i++;
    }
    pdata->time_to_die = ft_atol(v[2],&err);
    if (pdata->time_to_die <= 0 || err)
    {
        write(2, "Error: Invalid time to die\n", 28);
        ft_malloc(0,GB_CLEAR);
        return (1);
    }
    pdata->time_to_eat = ft_atol(v[3],&err);
    if (pdata->time_to_eat <= 0 || err)
    {
        write(2, "Error: Invalid time to eat\n", 28);
        ft_malloc(0,GB_CLEAR);
        return (1);
    }
    pdata->time_to_sleep = ft_atol(v[4],&err);
    if (pdata->time_to_sleep <= 0 || err)
    {
        write(2, "Error: Invalid time to sleep\n", 30);
        ft_malloc(0,GB_CLEAR);
        return (1);
    }
    pdata->start_time = get_time();
    pdata->is_dead = 0;
    pdata->is_all_full = 0;
    pthread_mutex_init(&pdata->print_mutex,NULL);
    pthread_mutex_init(&pdata->death_mutex,NULL);
    pthread_mutex_init(&pdata->is_full_mutex,NULL);

    if (c == 6)
    {
        pdata->number_of_times_to_eat = ft_atol(v[5],&err);
        if(pdata->number_of_times_to_eat <= 0 || err)
        {
            printf("Error: Invalid number of times to eat\n");
            ft_malloc(0,GB_CLEAR);
            return (1);
        }
    }
    else
        pdata->number_of_times_to_eat = -1;
    i = 0;
    while (i < pdata->noph)
    {
        philos[i].right_fork = &pdata->forks[i];
        philos[i].left_fork = &pdata->forks[(i + 1) % pdata->noph];
        philos[i].id = i + 1;
        philos[i].is_full = 0;
        philos[i].last_meal_time = get_time();
        pthread_mutex_init(&philos[i].meal_mutex, NULL);
        philos[i].number_of_times_eaten = 0;
        philos[i].pdata = pdata;
        i++;
    }
    i = 0;
    while (i < pdata->noph)
    {
        pthread_create((&philos[i].thread), NULL, philo, &philos[i]);
        i++;
    }
    monitor(philos);
    i = 0;
    while (i < pdata->noph)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    ft_malloc(0,GB_CLEAR);
}
