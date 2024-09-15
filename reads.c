/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reads.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:37:31 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/15 21:47:00 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	malloc_rules(t_data *data)
{
	int	i;
	int	j;
	int	sig;

	i = -1;
	data->lsim->rules = (float_t **)malloc(sizeof(float_t *) * (data->part_num + 1));
	if (!data->lsim->rules)
		display_error(data, "rules malloc error\n");
	while (++i < data->part_num)
	{
		j = -1;
		data->lsim->rules[i] = (float_t *)malloc(sizeof(float_t) * data->part_num);
		if (!data->lsim->rules[i])
			display_error(data, "rules malloc error\n");
		while (++j < data->part_num)
		{
			sig = 1;
			if (rand() % 2 == 0)
				sig = -1;
			data->lsim->rules[i][j] = rand_float_t(0.3, 2.0) * sig;
		}
	}
	data->lsim->rules[i] = NULL;
	print_rules(data, data->lsim->rules);
}

void	malloc_atrrules(t_data *data)
{
	int	i;
	int	j;
	int	min;
	int	max;

	min = 25 * data->radius;
	max = 80 * data->radius;
	i = -1;
	data->lsim->atrrules = (float_t **)malloc(sizeof(float_t *) * (data->part_num + 1));
	if (!data->lsim->atrrules)
		display_error(data, "rules malloc error\n");
	while (++i < data->part_num)
	{
		j = -1;
		data->lsim->atrrules[i] = (float_t *)malloc(sizeof(float_t) * data->part_num);
		if (!data->lsim->atrrules[i])
			display_error(data, "rules malloc error\n");
		while (++j < data->part_num)
			data->lsim->atrrules[i][j] = rand_float_t(min, max);
	}
	data->lsim->atrrules[i] = NULL;
	print_rules(data, data->lsim->atrrules);
}

void	malloc_reprules(t_data *data)
{
	int	i;
	int	j;
	int	min;
	int	max;

	min = 10 * data->radius;
	max = 17 * data->radius;
	i = -1;
	data->lsim->reprules = (float_t **)malloc(sizeof(float_t *) * (data->part_num + 1));
	if (!data->lsim->reprules)
		display_error(data, "rules malloc error\n");
	while (++i < data->part_num)
	{
		j = -1;
		data->lsim->reprules[i] = (float_t *)malloc(sizeof(float_t) * data->part_num);
		if (!data->lsim->reprules[i])
			display_error(data, "rules malloc error\n");
		while (++j < data->part_num)
			data->lsim->reprules[i][j] = rand_float_t(min, max);
	}
	data->lsim->reprules[i] = NULL;
	print_rules(data, data->lsim->reprules);
}

void	put_part_color(t_lifeform *tmp)
{
	if (tmp->id == 0)
		tmp->color = RED;
	if (tmp->id == 1)
		tmp->color = GREEN;
	if (tmp->id == 2)
		tmp->color = BLUE;
	if (tmp->id == 3)
		tmp->color = YELLOW;
	if (tmp->id == 4)
		tmp->color = PINK;
	if (tmp->id == 5)
		tmp->color = CYAN;
}

void	create_lsim(t_data *data)
{
	int			i;
	t_lifeform	*tmp;

	srand((unsigned int)time(NULL));
	malloc_rules(data);
	malloc_atrrules(data);
	malloc_reprules(data);
	data->lsim->life = (t_lifeform *)malloc(sizeof(t_lifeform));
	if (!data->lsim->life)
		display_error(data, "life malloc error\n");
	i = -1;
	tmp = data->lsim->life;
	while (++i < data->num_of_life)
	{
		tmp->pos = create_vector(rand() % data->winx, rand() % data->winy);
		tmp->vel = create_vector(0, 0);
		tmp->acel = create_vector(0, 0);
		tmp->mass = 5;
		tmp->r = data->radius;
		tmp->id = i % data->part_num;
		put_part_color(tmp);
		if (i + 1 == data->num_of_life)
			break ;
		tmp->next = (t_lifeform *)malloc(sizeof(t_lifeform));
		if (!tmp->next)
			display_error(data, "life malloc error\n");
		tmp = tmp->next;
	}
}
