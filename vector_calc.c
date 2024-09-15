/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:30:59 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/15 17:19:07 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

t_vector	create_vector(float_t x, float_t y)
{
	return ((t_vector){x, y});
}

t_vector	vectoradd(t_vector v1, t_vector v2)
{
	return (create_vector(v1.x + v2.x, v1.y + v2.y));
}

t_vector	vectorsub(t_vector v1, t_vector v2)
{
	return (create_vector(v1.x - v2.x, v1.y - v2.y));
}

t_vector	vectordot(t_vector v1, t_vector v2)
{
	return (create_vector(v1.x * v2.x, v1.y * v2.y));
}

t_vector	vectormult(t_vector v, float_t scalar)
{
	return (create_vector(v.x * scalar, v.y * scalar));
}

t_vector	vectordiv(t_vector v, float_t scalar)
{
	if (scalar != 0)
		return (create_vector(v.x / scalar, v.y / scalar));
	return (create_vector(v.x, v.y));
}

t_vector	vector_setmagdiv(t_vector v, float_t magnitude)
{
	float_t	mag;

	mag = sqrt(vector_magsq(v));
	if (magnitude != 0)
		return (vectordiv(v, mag / magnitude));
	return (v);
}

t_vector	vector_setmagmult(t_vector v, float_t magnitude)
{
	float_t	mag;

	mag = vector_magsqsqrt(v);
	if (mag != 0)
		return (vectormult(v, magnitude / mag));
	return (v);
}

float_t	vector_magsqsqrt(t_vector v)
{
	return (sqrtf((v.x * v.x) + (v.y * v.y)));
}

float_t	vector_magsq(t_vector v)
{
	return (v.x * v.x + v.y * v.y);
}

float_t	constrain_float_t(float_t val, float_t min, float_t max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}
