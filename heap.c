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
		if (left < heap->coders_in
			&& heap_compare(heap->coders[left],
				heap->coders[smallest], scheduler))
			smallest = left;
		if (right < heap->coders_in
			&& heap_compare(heap->coders[right],
				heap->coders[smallest], scheduler))
			smallest = right;
		if (smallest == i)
			break ;
		heap_swap(heap, i, smallest);
		i = smallest;
	}
}

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
