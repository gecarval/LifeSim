/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reads.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:37:31 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/13 19:03:33 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

float_t	rand_float_t(float_t min, float_t max)
{
	return (((float_t)rand() / ((float_t)RAND_MAX / (max - min)) + min));
}

void	print_rules(float_t **rules)
{
	int	i;
	int	j;

	i = -1;
	printf("	  RED	GREEN	BLUE	YELLOW	PINK	CYAN\n");
	while (++i < PART_NUM)
	{
		j = -1;
		if (i == 0)
		printf("RED	");
		if (i == 1)
		printf("GREEN	");
		if (i == 2)
		printf("BLUE	");
		if (i == 3)
		printf("YELLOW	");
		if (i == 4)
		printf("PINK	");
		if (i == 5)
		printf("CYAN	");
		printf("|");
		while (++j < 4)
		{
			printf(" %f |", rules[i][j]);
		}
			printf("\n");
	}
}

void	reset_rules(float_t **rules, int min, int max, int neg)
{
	int	i;
	int	j;
	int	sig;

	i = -1;
	while (++i < PART_NUM)
	{
		j = -1;
		while (++j < PART_NUM)
		{
			sig = 1;
			if (rand() % 2 == 0)
				sig = -1 * neg;
			rules[i][j] = rand_float_t(min, max) * sig;
		}
	}
	print_rules(rules);
}

void	malloc_rules(t_data *data)
{
	int	i;
	int	j;
	int		sig;

	i = -1;
	data->lsim->rules = (float_t **)malloc(sizeof(float_t *) * (PART_NUM));
	if (!data->lsim->rules)
		display_error(data, "rules malloc error\n");
	while (++i < PART_NUM)
	{
		j = -1;
		data->lsim->rules[i] = (float_t *)malloc(sizeof(float_t) * PART_NUM);
		if (!data->lsim->rules[i])
			display_error(data, "rules malloc error\n");
		while (++j < PART_NUM)
		{
			sig = 1;
			if (rand() % 2 == 0)
				sig = -1;
			data->lsim->rules[i][j] = rand_float_t(0.3, 1.0) * sig;
		}
	}
	data->lsim->rules[i] = NULL;
	print_rules(data->lsim->rules);
}

void	malloc_atrrules(t_data *data)
{
	int	i;
	int	j;
	int	min;
	int	max;

	min = 25 * RADIUS;
	max = 80 * RADIUS;
	i = -1;
	data->lsim->atrrules = (float_t **)malloc(sizeof(float_t *) * (PART_NUM + 1));
	if (!data->lsim->atrrules)
		display_error(data, "rules malloc error\n");
	while (++i < PART_NUM)
	{
		j = -1;
		data->lsim->atrrules[i] = (float_t *)malloc(sizeof(float_t) * PART_NUM);
		if (!data->lsim->atrrules[i])
			display_error(data, "rules malloc error\n");
		while (++j < PART_NUM)
			data->lsim->atrrules[i][j] = rand_float_t(min, max);
	}
	data->lsim->atrrules[i] = NULL;
	print_rules(data->lsim->atrrules);
}

void	malloc_reprules(t_data *data)
{
	int	i;
	int	j;
	int	min;
	int	max;

	min = 10 * RADIUS;
	max = 17 * RADIUS;
	i = -1;
	data->lsim->reprules = (float_t **)malloc(sizeof(float_t *) * (PART_NUM + 1));
	if (!data->lsim->reprules)
		display_error(data, "rules malloc error\n");
	while (++i < PART_NUM)
	{
		j = -1;
		data->lsim->reprules[i] = (float_t *)malloc(sizeof(float_t) * PART_NUM);
		if (!data->lsim->reprules[i])
			display_error(data, "rules malloc error\n");
		while (++j < PART_NUM)
			data->lsim->reprules[i][j] = rand_float_t(min, max);
	}
	data->lsim->reprules[i] = NULL;
	print_rules(data->lsim->reprules);
}

void	create_lsim(t_data *data)
{
	int		i;
	t_lifeform	*tmp;

	data->lsim->life = (t_lifeform *)malloc(sizeof(t_lifeform) * NUMBER_OF_LIFEFORM);
	if (!data->lsim->life)
		display_error(data, "life malloc error\n");
	data->lsim->g = 0.3;
	srand((unsigned int)time(NULL));
	malloc_rules(data);
	malloc_reprules(data);
	malloc_atrrules(data);
	i = -1;
	while (++i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		tmp->pos = (t_vector){rand() % WINDX, rand() % WINDY};
		tmp->vel = (t_vector){0, 0};
		tmp->acel = (t_vector){0, 0};
		tmp->mass = 5;
		tmp->r = RADIUS;
		tmp->id = i % PART_NUM;
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
	tmp = (data->lsim->life) + i;
	tmp = NULL;
}
