/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/11/08 09:09:58 by gecarval         ###   ########.fr       */
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
	render_background(data, 0x000000);
	while (++i < data->num_of_life)
	{
		j = tmp->r;
		if (j == 1)
		{
			pixel_to_img((int)tmp->pos.x, (int)tmp->pos.y, data, tmp->color);
			j--;
		}
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

void	attraction(t_lifeform *mover, t_lifeform *other, t_data *data,
		float_t d)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;

	if (d < data->lsim->atrrules[mover->id][other->id])
	{
		force = (t_vector){0, 0};
		force = vectorsub(mover->pos, other->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = data->lsim->rules[mover->id][other->id] * ((data->lsim->atrg
					* mover->mass) / dist);
		force = vector_setmagmult(force, strength);
		pthread_mutex_lock(&other->life_mutex);
		applyforce(other, force);
		pthread_mutex_unlock(&other->life_mutex);
	}
	// to other
	if (d < data->lsim->atrrules[other->id][mover->id])
	{
		force = (t_vector){0, 0};
		force = vectorsub(other->pos, mover->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = data->lsim->rules[other->id][mover->id] * ((data->lsim->atrg
					* other->mass) / dist);
		force = vector_setmagmult(force, strength);
		pthread_mutex_lock(&mover->life_mutex);
		applyforce(mover, force);
		pthread_mutex_unlock(&mover->life_mutex);
	}
}

void	repulsion(t_lifeform *mover, t_lifeform *other, t_data *data, float_t d)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;

	if (d < data->lsim->reprules[mover->id][other->id])
	{
		force = (t_vector){0, 0};
		force = vectorsub(mover->pos, other->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = -1 * ((data->lsim->repg * mover->mass) / dist);
		force = vector_setmagmult(force, strength);
		pthread_mutex_lock(&other->life_mutex);
		applyforce(other, force);
		pthread_mutex_unlock(&other->life_mutex);
	}
	if (d < data->lsim->reprules[other->id][mover->id])
	{
		force = (t_vector){0, 0};
		force = vectorsub(other->pos, mover->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = -1 * ((data->lsim->repg * other->mass) / dist);
		force = vector_setmagmult(force, strength);
		pthread_mutex_lock(&mover->life_mutex);
		applyforce(mover, force);
		pthread_mutex_unlock(&mover->life_mutex);
	}
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
		acx = mover->vel.x / 100;
	if (mover->vel.y != 0)
		acy = mover->vel.y / 100;
	mover->acel = create_vector(acx, acy);
	mover->vel = vectorsub(mover->vel, mover->acel);
}

void	limit_velocity(t_lifeform *mover, float_t limit)
{
	if (mover->vel.x > limit)
		mover->vel.x = limit;
	if (mover->vel.y > limit)
		mover->vel.y = limit;
	if (mover->vel.x < -limit)
		mover->vel.x = -limit;
	if (mover->vel.y < -limit)
		mover->vel.y = -limit;
}

void	update_position(t_lifeform *mover)
{
	mover->vel = vectoradd(mover->vel, mover->acel);
	limit_velocity(mover, 3000);
	fricction(mover);
	mover->pos = vectoradd(mover->pos, mover->vel);
	mover->acel = create_vector(0, 0);
}

void	process_velocity(t_data *data)
{
	int			i;
	t_lifeform	*tmp;

	i = -1;
	tmp = (data->lsim->life);
	while (++i < data->num_of_life)
	{
		tmp->prev_pos = tmp->pos;
		update_position(tmp);
		tmp->pos = limit_vector(tmp->pos, data);
		tmp = tmp->next;
	}
}

void	apply_collision_onquad(t_quadtree *qt, t_data *data)
{
	int			i;
	int			j;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;
	t_vector	dist;

	if (qt->divided)
	{
		apply_collision_onquad(qt->northwest, data);
		apply_collision_onquad(qt->northeast, data);
		apply_collision_onquad(qt->southwest, data);
		apply_collision_onquad(qt->southeast, data);
		return ;
	}
	i = -1;
	while (++i < qt->point_count)
	{
		j = i;
		while (++j < qt->point_count)
		{
			tmp = (qt->points[i]).life;
			tmp2 = (qt->points[j]).life;
			if (tmp != tmp2)
			{
				dist = vectorsub(tmp2->pos, tmp->pos);
				collision(tmp, tmp2, vector_magsqsqrt(dist));
			}
		}
	}
}

void	process_collision_quad(t_data *data)
{
	int			i;
	int			rp;
	t_point		pt;
	t_lifeform	*tmp;
	t_quadtree	*qt;

	rp = -1;
	while (++rp < COLLISION_STEPS)
	{
		i = -1;
		tmp = data->lsim->life;
		qt = create_quadtree_fromglobals(data->winx, data->winy);
		while (++i < data->num_of_life)
		{
			pt = create_point(tmp->pos.x, tmp->pos.y, tmp);
			insert_point(qt, pt);
			tmp = tmp->next;
		}
		apply_collision_onquad(qt, data);
		if (data->show_tree == 1)
			display_quadtree_boundaries(qt, data);
		free_quadtree(qt);
	}
}

void	life_sim(t_data *data)
{
	int	i;

	render_lifeform(data);
	process_velocity(data);
	process_collision_quad(data);
	i = 0;
	while (i < MAX_THREADS)
	{
		data->processor[i].data = data;
		data->processor[i].start = i * (data->num_of_life / MAX_THREADS);
		data->processor[i].end = (i + 1) * (data->num_of_life / MAX_THREADS);
		if (pthread_create(&data->processor[i].thread, NULL, process_physics,
				&data->processor[i]) != 0)
			display_error(data, "failed thread\n");
		i++;
	}
	i = 0;
	while (i < MAX_THREADS)
	{
		if (pthread_join(data->processor[i].thread, NULL) != 0)
			display_error(data, "failed thread\n");
		i++;
	}
}

void	*process_physics(void *ptr)
{
	int			i;
	int			j;
	float_t		d;
	t_vector	dist;
	t_lifeform	*tmp;
	t_lifeform	*tmp2;
	t_processor	*processor;

	i = 0;
	processor = (t_processor *)ptr;
	tmp = processor->data->lsim->life;
	while (i < processor->start)
	{
		tmp = tmp->next;
		i++;
	}
	while (i < processor->end)
	{
		j = i;
		tmp2 = tmp->next;
		while (++j < processor->data->num_of_life)
		{
			dist = vectorsub(tmp2->pos, tmp->pos);
			dist = mirror_forces(dist, processor->data);
			d = vector_magsqsqrt(dist);
			attraction(tmp, tmp2, processor->data, d);
			repulsion(tmp, tmp2, processor->data, d);
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
		i++;
	}
	return (NULL);
}
