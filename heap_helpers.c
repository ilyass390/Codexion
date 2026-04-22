/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_helpers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:02:29 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:03:08 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_compare(t_coder *a, t_coder *b, int scheduler)
{
	if (scheduler == FIFO)
		return (a->request_time < b->request_time);
	return (a->deadline < b->deadline);
}

void	heap_swap(t_heap *heap, int a, int b)
{
	t_coder	*tmp;

	tmp = heap->coders[a];
	heap->coders[a] = heap->coders[b];
	heap->coders[b] = tmp;
}

void	bubble_up(t_heap *heap, int i, int scheduler)
{
	int	parent;

	while (i > 0)
	{
		parent = (i - 1) / 2;
		if (heap_compare(heap->coders[i], heap->coders[parent], scheduler))
		{
			heap_swap(heap, i, parent);
			i = parent;
		}
		else
			break ;
	}
}

void	bubble_down(t_heap *heap, int i, int scheduler)
{
	int	left;
	int	right;
	int	smallest;

	while (1)
	{
		left = 2 * i + 1;
		right = 2 * i + 2;
		smallest = i;
		if (left < heap->coders_in && heap_compare(heap->coders[left],
				heap->coders[smallest], scheduler))
			smallest = left;
		if (right < heap->coders_in && heap_compare(heap->coders[right],
				heap->coders[smallest], scheduler))
			smallest = right;
		if (smallest == i)
			break ;
		heap_swap(heap, i, smallest);
		i = smallest;
	}
}
