/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_bonus.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zbouchra <zbouchra@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 19:29:47 by zbouchra          #+#    #+#             */
/*   Updated: 2025/04/25 16:49:52 by zbouchra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_collect	*add_back(t_collect *current, t_collect *new)
{
	current->next = new;
	return (new);
}

t_collect	*new_node(void *ptr)
{
	t_collect	*new;

	new = malloc(sizeof(t_collect));
	if (!new)
		return (NULL);
	new->content = ptr;
	new->next = NULL;
	return (new);
}

void	*ft_clear(t_collect **head)
{
	t_collect	*tmp;
	t_collect	*holder;

	holder = *head;
	while (holder)
	{
		tmp = (holder)->next;
		free((holder)->content);
		free(holder);
		holder = tmp;
	}
	*head = NULL;
	return (NULL);
}

void	*ft_malloc_(t_collect **head, t_collect **current, size_t size)
{
	void	*ptr;

	ptr = malloc(size);
	if (!ptr)
		return (NULL);
	*head = new_node(ptr);
	if (!*head)
		return (ft_clear(head));
	if (!(*head)->content)
		return (NULL);
	*current = *head;
	return (NULL);
}

void	*ft_malloc(size_t size, int flag)
{
	static t_collect	*current;
	static t_collect	*head;

	if (flag == 0)
	{
		if (head == NULL)
			ft_malloc_(&head, &current, size);
		else
		{
			current = add_back(current, new_node(malloc(size)));
			if (!current)
				return (ft_clear(&head));
		}
		return (current->content);
	}
	else if (flag == 2)
		return (malloc(size));
	return (ft_clear(&head));
}
