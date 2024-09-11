/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lifesim.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 12:18:49 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/11 18:29:59 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

t_vector	create_vector(float_t x, float_t y)
{
	return (t_vector){x, y};
}

t_vector	vector_sub(t_vector v1, t_vector v2)
{
	return (create_vector(v1.x - v2.x, v1.y - v2.y));
}

t_vector	vectordiv(t_vector v, float_t scalar)
{
	return (create_vector(v.x / scalar, v.y / scalar));
}

void	vectoradd(t_vector *v1, t_vector v2)
{
	v1->x += v2.x;
	v1->y += v2.y;
}

float_t	vector_magsq(t_vector v)
{
	return (v.x * v.x + v.y * v.y);
}

float_t	constrain(float_t val, float_t min, float_t max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}

t_vector	vector_setmag(t_vector v, float_t magnitude)
{
	float_t	mag;

	mag = sqrt(vector_magsq(v));
	if (mag != 0)
		return vectordiv(v, mag / magnitude);
	return (v);
}

void	applyforce(t_lifeform *mover, t_vector force)
{
	t_vector	f;

	f = vectordiv(force, mover->mass);
	vectoradd(&mover->acel, f);
}

void	attract(t_lifeform *mover, t_lifeform *other)
{
	t_vector	force;
	float_t		strength;
	float_t		dist;
	float_t		g;

	g = 1;
	force = vector_sub(mover->pos, other->pos);
	dist = constrain(vector_magsq(force), 100, 1000);
	strength = (g * mover->mass * other->mass) / dist;
	force = vector_setmag(force, strength);
	applyforce(other, force);
}

void	update_mover(t_lifeform *mover)
{
	vectoradd(&mover->vel, mover->acel);
	vectoradd(&mover->pos, mover->vel);
	mover->acel = create_vector(0, 0);
}

void	render_lifeform(t_data *data)
{
	int		i;
	int		j;
	t_lifeform	*tmp;

	i = 0;
	while (i < NUMBER_OF_LIFEFORM)
	{
		tmp = (data->lsim->life) + i;
		j = 4;
		while (--j >= 0)
		{
			circlebres((int)tmp->ppos.x, (int)tmp->ppos.y, j, data, 0x000000);
			circlebres((int)tmp->pos.x, (int)tmp->pos.y, j, data, tmp->color);
		}
		i++;
	}
}

void	process_attraction(t_data *data)
{
	int		i;
	int		j;
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
			if (i != j)
			{
				attract(tmp, tmp2);
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
