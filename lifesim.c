/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/13 14:45:06 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

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
	while (++i < NUMBER_OF_LIFEFORM)
	{
		j = -1;
		while (++j < NUMBER_OF_LIFEFORM)
		{
			tmp = (data->lsim->life) + i;
			tmp2 = (data->lsim->life) + j;
			res = vector_magsqsqrt(vector_sub(tmp2->pos, tmp->pos));
			if (i != j)
			{
				if (res < MAX_DIST)
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
//	render_attraction(data);
	while (++i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		j = RADIUS;
		while (--j >= 0)
			circlebres((int)tmp->pos.x, (int)tmp->pos.y, j, data, tmp->color);
	}
}

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

	g = 1;
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

	g = 3;
	force = vector_sub(mover->pos, other->pos);
	dist = constrain(vector_magsq(force), 100, 1000);
	strength = -1 * ((g * mover->mass * other->mass) / dist);
	force = vector_setmag(force, strength);
	applyforce(other, force);
}

void	collision(t_lifeform *p1, t_lifeform *p2)
{
	t_vector impactvector = vector_sub(p2->pos, p1->pos);
	float_t d = vector_magsqsqrt(impactvector);

	if (d < p1->r + p2->r)
	{
		float_t overlap = d - (p1->r + p2->r);
		t_vector dir = impactvector;
		setmag(&dir, overlap * 0.5);
		vectoradd(&(p1->pos), dir);
		dir = vectormult(dir, -1);
		vectoradd(&(p2->pos), dir);
		d = p1->r + p2->r;
		setmag(&impactvector, d);
		float_t msum = p1->mass + p2->mass;
		t_vector vdiff = vector_sub(p2->vel, p1->vel);
		float_t num = vdiff.x * impactvector.x + vdiff.y * impactvector.y;
		float_t den = msum * d * d;
		t_vector deltava = impactvector;
		deltava = vectormult(deltava, 0 * p2->mass * num / den);
		vectoradd(&(p1->vel), deltava);
		t_vector deltavb = impactvector;
		deltavb = vectormult(deltavb, -0 * p1->mass * num / den);
		vectoradd(&(p2->vel), deltavb);
	}
}
void	update_mover(t_lifeform *mover)
{
	float_t	acx;
	float_t	acy;

	acx = 0;
	acy = 0;
	vectoradd(&mover->vel, mover->acel);
	if (mover->vel.x > 0.7)
		mover->vel.x = 0.7;
	if (mover->vel.y > 0.7)
		mover->vel.y = 0.7;
	vectoradd(&mover->pos, mover->vel);
	if (mover->vel.x != 0 && mover->vel.y != 0)
	{
		if (mover->vel.x != 0)
		{
			acx = mover->vel.x / 75;
			mover->acel = create_vector(acx, acy);
		}
		if (mover->vel.y != 0)
		{
			acy = mover->vel.y / 75;
			mover->acel = create_vector(acx, acy);
		}
		mover->vel = vector_sub(mover->vel, mover->acel);
		vectoradd(&mover->pos, mover->vel);
	}
	mover->acel = create_vector(0, 0);
}

void	process_collision(t_data *data)
{
	int		i;
	int		j;
	int		rp;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;

	i = -1;
	while (++i < NUMBER_OF_LIFEFORM)
	{
		rp = -1;
		while (++rp < 8)
		{
			j = -1;
			while (++j < NUMBER_OF_LIFEFORM)
			{
				tmp = (data->lsim->life) + i;
				tmp2 = (data->lsim->life) + j;
				if (i != j)
					collision(tmp, tmp2);
			}
		}
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
			res = vector_magsqsqrt(vector_sub(tmp2->pos, tmp->pos));
			if (i != j)
			{
				if (res <  4 * RADIUS)
					repulsion(tmp, tmp2);
				if (res < MAX_DIST)
					attract(tmp, tmp2, data);
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
	process_attraction(data);
	process_velocity(data);
	//process_collision(data);
	render_lifeform(data);
}
