/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reads.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:37:31 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/12 21:04:47 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

float_t	rand_float_t(float_t min, float_t max)
{
	return ((float_t)(float_t)rand() / ((float_t)RAND_MAX / (max - min)));
}

void	print_rules(t_data *data)
{
	int	i;
	int	j;

	i = -1;
	printf("	  RED		GREEN	BLUE	YELLOW\n");
	while (++i < 4)
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
		printf("|");
		while (++j < 4)
		{
			printf(" %f |", data->lsim->rules[i][j]);
		}
			printf("\n");
	}
}

void	create_lsim(t_data *data)
{
	int		i;
	int		j;
	int		sig;
	t_lifeform	*tmp;

	data->lsim->life = (t_lifeform *)malloc(sizeof(t_lifeform) * NUMBER_OF_LIFEFORM);
	if (!data->lsim->life)
		display_error(data, "life malloc error\n");
	data->lsim->g = 0.3;
	data->lsim->rules = (float_t **)malloc(sizeof(float_t *) * 5);
	if (!data->lsim->life)
		display_error(data, "rules malloc error\n");
	srand((unsigned int)time(NULL));
	i = -1;
	while (++i < 4)
	{
		j = -1;
		data->lsim->rules[i] = (float_t *)malloc(sizeof(float_t) * 4);
		if (!data->lsim->rules[i])
			display_error(data, "rules malloc error\n");
		while (++j < 4)
		{
			sig = (rand() % 2) * -1;
			if (sig == 0)
				sig = 1;
			data->lsim->rules[i][j] = rand_float_t(0.0, 1.0) * sig;
		}
	}
/*	data->lsim->rules[0][0] = 1;
	data->lsim->rules[0][1] = 1;
	data->lsim->rules[0][2] = -1;
	data->lsim->rules[0][3] = -1;

	data->lsim->rules[1][0] = -1;
	data->lsim->rules[1][1] = 1;
	data->lsim->rules[1][2] = 1;
	data->lsim->rules[1][3] = -1;

	data->lsim->rules[2][0] = -1;
	data->lsim->rules[2][1] = -1;
	data->lsim->rules[2][2] = 1;
	data->lsim->rules[2][3] = 1;

	data->lsim->rules[3][0] = 1;
	data->lsim->rules[3][1] = -1;
	data->lsim->rules[3][2] = -1;
	data->lsim->rules[3][3] = 1;*/
	data->lsim->rules[i] = NULL;
	print_rules(data);
	i = -1;
	while (++i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		tmp->pos.x = rand() % WINDX;
		tmp->pos.y = rand() % WINDY;
		tmp->vel.x = rand() % 2;
		tmp->vel.y = rand() % 2;
		tmp->acel.x = 0;
		tmp->acel.y = 0;
		tmp->mass = 20;
		tmp->r = RADIUS;
		if (rand() % 2)
			tmp->vel.x *= -1;
		if (rand() % 2)
			tmp->vel.y *= -1;
		tmp->id = rand() % 4;
		if (tmp->id == 0)
			tmp->color = RED;
		if (tmp->id == 1)
			tmp->color = GREEN;
		if (tmp->id == 2)
			tmp->color = BLUE;
		if (tmp->id == 3)
			tmp->color = YELLOW;
	}
	tmp = (data->lsim->life) + i;
	tmp = NULL;
}
