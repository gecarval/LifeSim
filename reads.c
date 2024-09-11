/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reads.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:37:31 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/11 18:04:30 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	create_lsim(t_data *data)
{
	int		i;
	t_lifeform	*tmp;

	data->lsim->life = (t_lifeform *)malloc(sizeof(t_lifeform) * NUMBER_OF_LIFEFORM);
	if (!data->lsim->life)
		display_error(data, "life malloc error\n");
	i = 0;
	srand((unsigned int)time(NULL));
	data->lsim->g = 0.3;
	while (i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		tmp->pos.x = rand() % WINDX;
		tmp->pos.y = rand() % WINDY;
		tmp->vel.x = 0;
		tmp->vel.y = 0;
		tmp->acel.x = 0;
		tmp->acel.y = 0;
		tmp->mass = 10;
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
		i++;
	}
}
