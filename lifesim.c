/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/12 15:40:11 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	applyforce(t_lifeform *mover, t_vector force)
{
	t_vector	f;

	f = vectordiv(force, mover->mass);
	vectoradd(&mover->acel, f);
}

void	attract(t_lifeform *mover, t_lifeform *other, t_data *data)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	g = 0.35;
	force = vector_sub(mover->pos, other->pos);
	dist = constrain(vector_magsq(force), 100, 1000);
	strength = data->lsim->rules[mover->id][other->id] * ((g * mover->mass * other->mass) / dist);
	force = vector_setmag(force, strength);
	applyforce(other, force);
}

void	repulsion(t_lifeform *mover, t_lifeform *other)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	g = 0.35;
	force = vector_sub(mover->pos, other->pos);
	dist = constrain(vector_magsq(force), 100, 1000);
	strength = -1 * ((g * mover->mass * other->mass) / dist);
	force = vector_setmag(force, strength);
	applyforce(other, force);
}
void	update_mover(t_lifeform *mover)
{
	float_t	acx;
	float_t	acy;

	acx = 0;
	acy = 0;
	vectoradd(&mover->vel, mover->acel);
	vectoradd(&mover->pos, mover->vel);
	if (mover->vel.x != 0 && mover->vel.y != 0)
	{
		if (mover->vel.x != 0)
		{
			acx = mover->vel.x / 50;
			mover->acel = create_vector(acx, acy);
		}
		if (mover->vel.y != 0)
		{
			acy = mover->vel.y / 50;
			mover->acel = create_vector(acx, acy);
		}
		mover->vel = vector_sub(mover->vel, mover->acel);
		vectoradd(&mover->pos, mover->vel);
	}
	mover->acel = create_vector(0, 0);
}

void	render_attraction(t_data *data)
{
	int		i;
	int		j;
	float_t		res;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;
	t_delta		xd;
	t_delta		yd;

	i = -1;
	render_background(data, 0x000000);
	while (++i < NUMBER_OF_LIFEFORM)
	{
		j = -1;
		while (++j < NUMBER_OF_LIFEFORM)
		{
			tmp = (data->lsim->life) + i;
			tmp2 = (data->lsim->life) + j;
			res = fabsf((tmp->pos.x - tmp2->pos.x)) + fabsf((tmp->pos.y - tmp2->pos.y));
			if (i != j)
			{
				if (res > 0 && res < 40)
				{
					xd = (t_delta){tmp->pos.x - 1, tmp2->pos.x - 1};
					yd = (t_delta){tmp->pos.y - 1, tmp2->pos.y - 1};
					if (data->lsim->rules[tmp->id][tmp2->id] < 0)
						draw_line(xd, yd, data, 0x770000);
					xd = (t_delta){tmp->pos.x + 1, tmp2->pos.x + 1};
					yd = (t_delta){tmp->pos.y + 1, tmp2->pos.y + 1};
					if (data->lsim->rules[tmp->id][tmp2->id] > 0)
						draw_line(xd, yd, data, 0x000077);
				}
			}
		}
	}
}

void	render_lifeform(t_data *data)
{
	int		i;
	int		j;
	t_lifeform	*tmp;

	i = -1;
	render_background(data, 0x000000);
	render_attraction(data);
	while (++i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		j = 3;
		while (--j >= 0)
			circlebres((int)tmp->pos.x, (int)tmp->pos.y, j, data, tmp->color);
	}
}

void	process_attraction(t_data *data)
{
	int		i;
	int		j;
	float_t		res;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;

	i = -1;
	while (++i < NUMBER_OF_LIFEFORM)
	{
		j = -1;
		while (++j < NUMBER_OF_LIFEFORM)
		{
			tmp = (data->lsim->life) + i;
			tmp2 = (data->lsim->life) + j;
			res = fabsf((tmp->pos.x - tmp2->pos.x)) + fabsf((tmp->pos.y - tmp2->pos.y));
			if (i != j)
			{
				if (res >= 0 && res <= 6)
				{
					repulsion(tmp, tmp2);
				}
				else if (res > 6 && res < 50)
				{
					attract(tmp, tmp2, data);
				}
			}
		}
	}
}

void	process_velocity(t_data *data)
{
	int		i;
	t_lifeform	*tmp;

	i = 0;
	while (i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		tmp->ppos.x = tmp->pos.x;
		tmp->ppos.y = tmp->pos.y;
		update_mover(tmp);
		if (tmp->pos.x > WINDX)
			tmp->pos.x = 0;
		if (tmp->pos.y > WINDY)
			tmp->pos.y = 0;
		if (tmp->pos.x < 0)
			tmp->pos.x = WINDX;
		if (tmp->pos.y < 0)
			tmp->pos.y = WINDY;
		i++;
	}
}

void	life_sim(t_data *data)
{
	process_velocity(data);
	process_attraction(data);
	render_lifeform(data);
}
