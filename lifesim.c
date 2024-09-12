/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/12 21:05:42 by gecarval         ###   ########.fr       */
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

/*
void collide(t_lifeform *p1, t_lifeform *p2)
{
    t_vector impactvector = vector_sub(p2->pos, p1->pos);
    float_t d = vector_magsqsqrt(impactvector);
    
    if (d < p1->r + p2->r)
    {
        float_t overlap = d - (p1->r + p2->r);
        t_vector dir = impactvector;
        setmag(&dir, overlap * 0.5);
        vectoradd(&(p1->position), dir);
        multVector(&dir, -1);
        addVector(&(p2->position), dir);

        d = p1->r + p2->r;
        setMag(&impactVector, d);

        float mSum = p1->mass + p2->mass;
        Vector2 vDiff = subVector(p2->velocity, p1->velocity);
        float num = vDiff.x * impactVector.x + vDiff.y * impactVector.y;
        float den = mSum * d * d;

        Vector2 deltaVA = impactVector;
        multVector(&deltaVA, 2 * p2->mass * num / den);
        addVector(&(p1->velocity), deltaVA);

        Vector2 deltaVB = impactVector;
        multVector(&deltaVB, -2 * p1->mass * num / den);
        addVector(&(p2->velocity), deltaVB);
    }
}
// Function to compute the magnitude of a vector
float magVector(Vector2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}*/

void	collision(t_lifeform *collider_a, t_lifeform *collider_b)
{
	t_vector	dir;
	t_vector	dpos;
	t_vector	diffv;
	t_vector	dvela;
	t_vector	dvelb;
	float_t		num;
	float_t		den;
	
	dpos = vector_sub(collider_b->pos, collider_a->pos);
	setmag(&dir, (vector_magsqsqrt(dpos) - (RADIUS + RADIUS)) * 0.5);
	vectoradd(&collider_a->pos, dir);
	dir = vectormult(dir, -1);
	vectoradd(&collider_b->pos, dir);	

	setmag(&dpos, (RADIUS + RADIUS));

	diffv = vector_sub(collider_b->vel, collider_a->vel);
	num = (diffv.x * dpos.x + diffv.y * dpos.y);
	den = ((collider_a->mass + collider_b->mass) * ((RADIUS + RADIUS) * (RADIUS + RADIUS)));

	dvela = vectormult(dpos, 2 * collider_b->mass * num / den);
	vectoradd(&(collider_a->vel), dvela);

	dvelb = vectormult(dpos, -2 * collider_a->mass * num / den);
	vectoradd(&(collider_b->vel), dvelb);
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
	strength = -4 * ((g * mover->mass * other->mass) / dist);
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
	while (++i < NUMBER_OF_LIFEFORM)
	{
		j = -1;
		while (++j < NUMBER_OF_LIFEFORM)
		{
			tmp = (data->lsim->life) + i;
			tmp2 = (data->lsim->life) + j;
			res = fabsf(tmp->pos.x - tmp2->pos.x) + fabsf(tmp->pos.y - tmp2->pos.y);
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
		j = RADIUS;
		while (--j >= 0)
		{
			circlebres((int)tmp->pos.x, (int)tmp->pos.y, j, data, tmp->color);
			if (j == RADIUS - 1)
				circlebres((int)tmp->pos.x, (int)tmp->pos.y, j, data, 0x777777);
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
			res = fabsf(tmp->pos.x - tmp2->pos.x) + fabsf(tmp->pos.y - tmp2->pos.y);
			if (i != j)
			{
				if (res < RADIUS + RADIUS)
				{
					collision(tmp, tmp2);
					//repulsion(tmp, tmp2);
				}
				else if (res >= RADIUS + RADIUS && res < 50)
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
