/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynarray_mem.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/10 18:04:09 by gedemais          #+#    #+#             */
/*   Updated: 2023/05/01 19:58:45 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

static int		realloc_content(t_dynarray *arr)
{
	void	*tmp;
	int		prev_size;

	prev_size = arr->byte_size;
	arr->byte_size += arr->byte_size;
	free(arr->tmp);
	if (!(tmp = ft_memalloc(arr->byte_size))
		|| !(arr->tmp = ft_memalloc(arr->byte_size)))
		return (-1);
	memcpy(tmp, arr->arr, prev_size);
	free(arr->arr);
	arr->arr = tmp;
	return (0);
}

int				check_space(t_dynarray *arr)
{
	while ((arr->nb_cells + 1) * arr->cell_size >= arr->byte_size)
		if (realloc_content(arr))
			return (-1);
	return (0);
}
