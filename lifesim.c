/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/16 19:09:39 by gecarval         ###   ########.fr       */
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
				dist = mirror_forces(dist, data);
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
	while (++i < data->num_of_life)
	{
		j = tmp->r;
		while (--j >= 0)
			circlebres((int)tmp->prev_pos.x, (int)tmp->prev_pos.y, j, data, 0x000000);
		tmp = tmp->next;
	}
	i = -1;
	tmp = data->lsim->life;
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

void	attraction(t_lifeform *mover, t_lifeform *other, t_data *data, float_t d)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	if (d < data->lsim->atrrules[mover->id][other->id])
	{
		g = 1;
		force = (t_vector){0, 0};
		force = vectorsub(mover->pos, other->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = data->lsim->rules[mover->id][other->id] * ((g * mover->mass * other->mass) / dist);
		force = vector_setmagmult(force, strength);
		applyforce(other, force);
	}
}

int	repulsion(t_lifeform *mover, t_lifeform *other, t_data *data, float_t d)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	if (d < data->lsim->reprules[mover->id][other->id])
	{
		g = 3;
		force = (t_vector){0, 0};
		force = vectorsub(mover->pos, other->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = -1 * ((g * mover->mass * other->mass) / dist);
		force = vector_setmagmult(force, strength);
		applyforce(other, force);
		return (1);
	}
	return (0);
}

void	collision(t_lifeform *p1, t_lifeform *p2, float_t d)
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

	if (d < p1->r + p2->r)
	{
		impactvector = vectorsub(p2->pos, p1->pos);
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
	if (mover->vel.x != 0)
		acx = mover->vel.x / 75;
	if (mover->vel.y != 0)
		acy = mover->vel.y / 75;
	mover->acel = create_vector(acx, acy);
	mover->vel = vectorsub(mover->vel, mover->acel);
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

void	process_velocity(t_data *data)
{
	int			i;
	t_lifeform	*tmp;

	i = 0;
	tmp = (data->lsim->life);
	while (i < data->num_of_life)
	{
		tmp->prev_pos = tmp->pos;
		update_position(tmp);
		tmp->pos = limit_vector(tmp->pos, data);
		tmp = tmp->next;
		i++;
	}
}

void	*process_physics2(void *ptr, t_data *data)
{
	int			i;
	int			j;
	float_t		d;
	t_vector	dist;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;

	i = -1;
	tmp = data->lsim->life;
	while (++i < data->num_of_life / 2)
		tmp = tmp->next;
	while (++i < data->num_of_life)
	{
		j = -1;
		tmp2 = data->lsim->life;
		while (++j < data->num_of_life)
		{
			if (tmp != tmp2)
			{
				dist = vectorsub(tmp2->pos, tmp->pos);
				dist = mirror_forces(dist, data);
				d = vector_magsqsqrt(dist);
				attraction(tmp, tmp2, data, d);
				d = repulsion(tmp, tmp2, data, d);
				if (d == 1)
				{
					dist = vectorsub(tmp2->pos, tmp->pos);
					collision(tmp, tmp2, vector_magsqsqrt(dist));
				}
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	(void)ptr;
	return (NULL);
}

void	*process_physics(void *ptr)
{
	int			i;
	int			j;
	float_t		d;
	t_vector	dist;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;
	t_data		*data;

	i = -1;
	data = (t_data *)ptr;
	tmp = data->lsim->life;
	while (++i < data->num_of_life / 2)
	{
		j = -1;
		tmp2 = data->lsim->life;
		while (++j < data->num_of_life)
		{
			if (tmp != tmp2)
			{
				dist = vectorsub(tmp2->pos, tmp->pos);
				collision(tmp, tmp2, vector_magsqsqrt(dist));
				dist = mirror_forces(dist, data);
				d = vector_magsqsqrt(dist);
				attraction(tmp, tmp2, data, d);
				repulsion(tmp, tmp2, data, d);
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
	}
	return (NULL);
}

void	life_sim(t_data *data)
{
	pthread_t	processor;

	render_lifeform(data);
	process_velocity(data);
	if (pthread_create(&processor, NULL, process_physics, data) != 0)
		display_error(data, "failed thread\n");
	process_physics2(NULL, data);
	if (pthread_join(processor, NULL) != 0)
		display_error(data, "failed thread\n");
}
