/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynarray.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maboye <maboye@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/08 19:13:14 by maboye            #+#    #+#             */
/*   Updated: 2022/08/26 00:24:14 by gedemais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DYNARRAY_H
# define DYNARRAY_H

typedef struct	s_dynarray
{
	void		*arr;
	void		*tmp;
	int			cell_size;
	int			nb_cells;
	int			byte_size;
	char		pad[4];
}				t_dynarray;

void			*dyacc(t_dynarray *arr, int index);
void			dynarray_free(t_dynarray *arr);
int				dynarray_init(t_dynarray *arr, int cell_size, int nb_cells);

int				dynarray_insert(t_dynarray *arr, void *src, int index);
int				dynarray_extract(t_dynarray *arr, int index);

int				check_space(t_dynarray *arr);

void			dynarray_bubblesort(t_dynarray *arr,
					int (*compare)(void *a, void *b));
void			dynarray_quicksort(t_dynarray *arr, int low, int high,
					int (*compare)(void *a, void *b));

void			dynarray_pop(t_dynarray *arr, bool front);
int				dynarray_push(t_dynarray *arr, void *src, bool front);
void			dynarray_replace_cell(t_dynarray *arr, void *src, int index);
void			dynarray_swap_cells(t_dynarray *add, int index1, int index2);

#endif
