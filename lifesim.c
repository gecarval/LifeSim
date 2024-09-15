/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/15 18:14:45 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	render_attraction(t_data *data)
{
	int			i;
	int			j;
	t_delta		xd;
	t_delta		yd;
	t_vector	dist;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;

	i = -1;
	tmp = data->lsim->life;
	while (++i < data->num_of_life)
	{
		j = -1;
		tmp2 = data->lsim->life;
		while (++j < data->num_of_life)
		{
			if (i != j)
			{
				dist = vectorsub(tmp2->pos, tmp->pos);
				if (dist.x > 0.5 * data->winx)
					dist.x -= data->winx;
				if (dist.x < -0.5 * data->winx)
					dist.x += data->winx;
				if (dist.y > 0.5 * data->winy)
					dist.y -= data->winy;
				if (dist.y < -0.5 * data->winy)
					dist.y += data->winy;
				if (vector_magsqsqrt(dist) < data->lsim->atrrules[tmp->id][tmp2->id])
				{
					xd = (t_delta){tmp->pos.x - 1, tmp2->pos.x - 1};
					yd = (t_delta){tmp->pos.y - 1, tmp2->pos.y - 1};
					if (data->lsim->rules[tmp->id][tmp2->id] < 0)
						draw_line(xd, yd, data, 0x770000);
					xd = (t_delta){tmp->pos.x + 1, tmp2->pos.x + 1};
					yd = (t_delta){tmp->pos.y + 1, tmp2->pos.y + 1};
					if (data->lsim->rules[tmp->id][tmp2->id] > 0)
						draw_line(xd, yd, data, 0x007700);
				}
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
}

void	render_lifeform(t_data *data)
{
	int			i;
	int			j;
	t_lifeform	*tmp;

	i = -1;
	tmp = data->lsim->life;
	render_background(data, 0x000000);
	while (++i < data->num_of_life)
	{
		j = tmp->r;
		while (--j >= 0)
			circlebres((int)tmp->pos.x, (int)tmp->pos.y, j, data, tmp->color);
		tmp = tmp->next;
	}
}

void	applyforce(t_lifeform *mover, t_vector force)
{
	t_vector	f;

	f = vectordiv(force, mover->mass);
	mover->acel = vectoradd(mover->acel, f);
}

void	attraction(t_lifeform *mover, t_lifeform *other, t_data *data)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	g = 1;
	force = (t_vector){0, 0};
	force = vectorsub(mover->pos, other->pos);
	if (force.x > 0.5 * data->winx)
		force.x -= data->winx;
	if (force.x < -0.5 * data->winx)
		force.x += data->winx;
	if (force.y > 0.5 * data->winy)
		force.y -= data->winy;
	if (force.y < -0.5 * data->winy)
		force.y += data->winy;
	dist = constrain_float_t(vector_magsq(force), 100, 1000);
	if (vector_magsqsqrt(force) < data->lsim->atrrules[mover->id][other->id])
	{
		strength = data->lsim->rules[mover->id][other->id] * ((g * mover->mass * other->mass) / dist);
		force = vector_setmagdiv(force, strength);
		applyforce(other, force);
	}
}

void	repulsion(t_lifeform *mover, t_lifeform *other, t_data *data)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	g = 3;
	force = (t_vector){0, 0};
	force = vectorsub(mover->pos, other->pos);
	if (force.x > 0.5 * data->winx)
		force.x -= data->winx;
	if (force.x < -0.5 * data->winx)
		force.x += data->winx;
	if (force.y > 0.5 * data->winy)
		force.y -= data->winy;
	if (force.y < -0.5 * data->winy)
		force.y += data->winy;
	dist = constrain_float_t(vector_magsq(force), 100, 1000);
	if (vector_magsqsqrt(force) < data->lsim->reprules[mover->id][other->id])
	{
		strength = -1 * ((g * mover->mass * other->mass) / dist);
		force = vector_setmagdiv(force, strength);
		applyforce(other, force);
	}
}

void	collision(t_lifeform *p1, t_lifeform *p2)
{
	t_vector	impactvector;
	t_vector	deltava;
	t_vector	deltavb;
	t_vector	vdiff;
	t_vector	dir;
	float_t		overlap;
	float_t		msum;
	float_t		num;
	float_t		den;
	float_t		d;

	impactvector = vectorsub(p2->pos, p1->pos);
	d = vector_magsqsqrt(impactvector);
	if (d < p1->r + p2->r)
	{
		overlap = d - (p1->r + p2->r);
		dir = impactvector;
		dir = vector_setmagmult(dir, overlap * 0.5);
		p1->pos = vectoradd(p1->pos, dir);
		dir = vectormult(dir, -1);
		p2->pos = vectoradd(p2->pos, dir);
		d = p1->r + p2->r;
		impactvector = vector_setmagmult(impactvector, d);
		msum = p1->mass + p2->mass;
		vdiff = vectorsub(p2->vel, p1->vel);
		num = vdiff.x * impactvector.x + vdiff.y * impactvector.y;
		den = msum * d * d;
		deltava = impactvector;
		deltava = vectormult(deltava, 1 * p2->mass * num / den);
		p1->vel = vectoradd(p1->vel, deltava);
		deltavb = impactvector;
		deltavb = vectormult(deltavb, -1 * p1->mass * num / den);
		p2->vel = vectoradd(p2->vel, deltavb);
	}
}

void	fricction(t_lifeform *mover)
{
	float_t	acx;
	float_t	acy;

	acx = 0;
	acy = 0;
	if (mover->vel.x != 0 || mover->vel.y != 0)
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
		mover->vel = vectorsub(mover->vel, mover->acel);
	}
}

void	limit_velocity(t_lifeform *mover, float_t limit)
{
	if (mover->vel.x > limit)
		mover->vel.x = limit;
	if (mover->vel.y > limit)
		mover->vel.y = limit;
}

void	update_position(t_lifeform *mover)
{
	mover->vel = vectoradd(mover->vel, mover->acel);
	limit_velocity(mover, 0.6);
	fricction(mover);
	mover->pos = vectoradd(mover->pos, mover->vel);
	mover->acel = create_vector(0, 0);
}

void	process_collision(t_data *data)
{
	int			i;
	int			j;
	int			rp;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;

	i = -1;
	tmp = data->lsim->life;
	while (++i < data->num_of_life)
	{
		rp = -1;
		while (++rp < COLLISION_STEPS)
		{
			j = -1;
			tmp2 = data->lsim->life;
			while (++j < data->num_of_life)
			{
				if (tmp != tmp2)
					collision(tmp, tmp2);
				tmp2 = tmp2->next;
			}
		}
		tmp = tmp->next;
	}
}

void	process_attraction(t_data *data)
{
	int			i;
	int			j;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;

	i = -1;
	tmp = data->lsim->life;
	while (++i < data->num_of_life)
	{
		j = -1;
		tmp2 = data->lsim->life;
		while (++j < data->num_of_life)
		{
			if (tmp != tmp2)
			{
				repulsion(tmp, tmp2, data);
				attraction(tmp, tmp2, data);
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
}

void	process_velocity(t_data *data)
{
	int			i;
	t_lifeform	*tmp;

	i = 0;
	tmp = (data->lsim->life);
	while (i < data->num_of_life)
	{
		update_position(tmp);
		if (tmp->pos.x > data->winx)
			tmp->pos.x = 0;
		if (tmp->pos.y > data->winy)
			tmp->pos.y = 0;
		if (tmp->pos.x < 0)
			tmp->pos.x = data->winx;
		if (tmp->pos.y < 0)
			tmp->pos.y = data->winy;
		tmp = tmp->next;
		i++;
	}
}

void	life_sim(t_data *data)
{
	process_attraction(data);
	process_collision(data);
	process_velocity(data);
	render_lifeform(data);
}
