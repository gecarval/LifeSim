/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_calc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:30:59 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/11 19:45:11 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

t_vector	create_vector(float_t x, float_t y)
{
	return ((t_vector){x, y});
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
