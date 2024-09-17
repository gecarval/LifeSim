/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/17 21:11:15 by gecarval         ###   ########.fr       */
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
		//data->lsim->rules[mover->id][other->id]
		strength = 1 * ((g * mover->mass * other->mass) / dist);
		force = vector_setmagmult(force, strength);
		applyforce(other, force);
		// to other
		force = (t_vector){0, 0};
		force = vectorsub(other->pos, mover->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = 1 * ((g * other->mass * mover->mass) / dist);
		force = vector_setmagmult(force, strength);
		applyforce(mover, force);
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
		// to other
		force = (t_vector){0, 0};
		force = vectorsub(other->pos, mover->pos);
		force = mirror_forces(force, data);
		dist = constrain_float_t(vector_magsq(force), 100, 1000);
		strength = -1 * ((g * other->mass * mover->mass) / dist);
		force = vector_setmagmult(force, strength);
		applyforce(mover, force);
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

void	apply_attraction_onquad(t_lifeform *m, t_quadtree* qtree, t_data *data)
{
	t_lifeform	*p;
	t_lifeform	temp;
	t_vector	qdist;
	float_t 	d;
	int		i;

	qdist = create_vector(qtree->boundary.x, qtree->boundary.y);
	qdist = vectorsub(m->pos, qdist);
	mirror_forces(qdist, data);
	d = vector_magsqsqrt(qdist);
	if (d < 250)
	{
		if (qtree->points != NULL)
		{
			i = -1;
			while (++i < qtree->point_count)
			{
				p = qtree->points[i].life;
				if (m != p)
				{
					qdist = vectorsub(m->pos, p->pos);
					mirror_forces(qdist, data);
					d = vector_magsqsqrt(qdist);
					attraction(m, p, data, d);
					repulsion(m, p, data, d);
				}
			}
		}
	}
	else
	{
		if (qtree->points != NULL)
		{
			temp.pos.x = qtree->boundary.x;
			temp.pos.y = qtree->boundary.y;
			temp.mass = m->mass * qtree->point_count;
			temp.id = 1;
			qdist = vectorsub(m->pos, temp.pos);
			mirror_forces(qdist, data);
			d = vector_magsqsqrt(qdist);
			attraction(m, &temp, data, d);
			repulsion(m, &temp, data, d);
		}
	}
	if (qtree->divided)
	{
		apply_attraction_onquad(m, qtree->northeast, data);
		apply_attraction_onquad(m, qtree->northwest, data);
		apply_attraction_onquad(m, qtree->southeast, data);
		apply_attraction_onquad(m, qtree->southwest, data);
	}
}

void	apply_collision_onquad(t_quadtree *qt, t_data *data)
{
	int	i;
	int	j;
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
		j = i + 1;
		while (++j < qt->point_count)
		{
			if (i != j)
			{
				tmp = (qt->points[i]).life;
				tmp2 = (qt->points[j]).life;
				dist = vectorsub(tmp2->pos, tmp->pos);
				collision(tmp, tmp2, vector_magsqsqrt(dist));
			}
		}
	}
}

void	draw_rectangle(t_rectangle r, t_data *data)
{
	t_delta	dx;
	t_delta	dy;

	defdel(&dx, r.left, r.right);
	defdel(&dy, r.top, r.top);
	draw_line(dx, dy, data, 0xFFFFFF);
	defdel(&dy, r.bottom, r.bottom);
	draw_line(dx, dy, data, 0xFFFFFF);
	defdel(&dx, r.left, r.left);
	defdel(&dy, r.top, r.bottom);
	draw_line(dx, dy, data, 0xFFFFFF);
	defdel(&dx, r.right, r.right);
	draw_line(dx, dy, data, 0xFFFFFF);
}

void	process_physics_quad(t_data *data)
{
	int			i;
	int			j;
	t_point		pt;
	t_lifeform	*tmp;
	t_quadtree	*qt;

	i = -1;
	tmp = data->lsim->life;
	qt = create_quadtree_fromglobals(data->winx, data->winy);
	while (++i < data->num_of_life)
	{
		pt = create_point(tmp->pos.x, tmp->pos.y, tmp);
		insert_point(qt, pt);
		tmp = tmp->next;
	}
	i = -1;
	tmp = data->lsim->life;
	while (++i < data->num_of_life)
	{
		apply_attraction_onquad(tmp, qt, data);
		tmp = tmp->next;
	}
	apply_collision_onquad(qt, data);
	// QUERY
	t_rectangle	range;
	t_point		*found[1000];
	int		found_count;
	range = create_rectangle(450, 450, 180, 180);
	draw_rectangle(range, data);
	query_quadtree(qt, &range, found, &found_count);
	i = -1;
	while (++i < found_count)
	{
		j = 4;
		while (--j >= 0)
			circlebres((int)found[i]->x, (int)found[i]->y, j, data, 0xffffff);
	}
	printf("%d\n", found_count);
	found_count = 0;
	// END
	display_quadtree_boundaries(qt, data);
	free_quadtree(qt);
}

void	life_sim(t_data *data)
{
	render_lifeform(data);
	process_velocity(data);
	process_physics_quad(data);
}

void	process_physics2(t_data *data)
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
		j = i + 1;
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
			}
			tmp2 = tmp2->next;
		}
		tmp = tmp->next;
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
	t_data		*data;

	i = -1;
	data = (t_data *)ptr;
	tmp = data->lsim->life;
	while (++i < data->num_of_life / 2)
	{
		j = i + 1;
		tmp2 = data->lsim->life;
		while (++j < data->num_of_life)
		{
			if (tmp != tmp2)
			{
				dist = vectorsub(tmp2->pos, tmp->pos);
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
