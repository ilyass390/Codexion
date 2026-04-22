/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap_func.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:43:30 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:03:14 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_push(t_heap *heap, t_coder *coder, int scheduler)
{
	if (heap->coders_in >= HEAP_MAX)
		return (-1);
	heap->coders[heap->coders_in] = coder;
	heap->coders_in++;
	bubble_up(heap, heap->coders_in - 1, scheduler);
	return (0);
}

t_coder	*heap_peek(t_heap *heap)
{
	if (heap->coders_in == 0)
		return (NULL);
	return (heap->coders[0]);
}

void	heap_pop(t_heap *heap, int scheduler)
{
	if (heap->coders_in == 0)
		return ;
	heap->coders[0] = heap->coders[heap->coders_in - 1];
	heap->coders_in--;
	if (heap->coders_in > 0)
		bubble_down(heap, 0, scheduler);
}

void	heap_destroy(t_heap *heap)
{
	if (heap->coders)
	{
		free(heap->coders);
		heap->coders = NULL;
	}
	heap->coders_in = 0;
}

void	heap_remove(t_heap *heap, t_coder *coder, int scheduler)
{
	int	i;

	i = 0;
	while (i < heap->coders_in)
	{
		if (heap->coders[i] == coder)
		{
			heap->coders[i] = heap->coders[heap->coders_in - 1];
			heap->coders_in--;
			if (i < heap->coders_in)
			{
				bubble_up(heap, i, scheduler);
				bubble_down(heap, i, scheduler);
			}
			return ;
		}
		i++;
	}
}
