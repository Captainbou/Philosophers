/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:21:03 by zbouchra          #+#    #+#             */
/*   Updated: 2025/05/02 13:35:02 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_pdata
{
	int					is_all_full;
	int					is_dead;
	int					noph;
	int					number_of_times_to_eat;
	long				start_time;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	pthread_mutex_t		death_mutex;
	pthread_mutex_t		print_mutex;
	pthread_mutex_t		is_full_mutex;
	pthread_mutex_t		start;
	pthread_mutex_t		*forks;
}						t_pdata;

typedef struct s_philo
{
	int					id;
	long				last_meal_time;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	pthread_mutex_t		meal_mutex;
	int					is_full;
	pthread_t			thread;
	long				number_of_times_eaten;
	t_pdata				*pdata;
}						t_philo;

#define NO_GB 2
#define GB 0
#define GB_CLEAR 1

typedef struct s_collect
{
	void				*content;
	struct s_collect	*next;
}						t_collect;

long					ft_atol(const char *str, int *err);
void					*ft_malloc(size_t size, int flag);
t_collect				*add_back(t_collect *current, t_collect *new);
t_collect				*new_node(void *ptr);
int						ft_strncmp(const char *s1, const char *s2, int n);
int						ft_strlen(const char *s);
int						init_data(t_philo **philos, t_pdata **pdata, int c,
							char **v);
int						init_philos_data(t_pdata *pdata, t_philo *philos);
void					*philo(void *param);
int						ft_destroy(t_pdata *pdata, char *err_msg);
long					get_time(void);
int						check_death(t_pdata *data);
void					print_message(t_philo *philos, char *message);
int						monitor(t_philo *philos);