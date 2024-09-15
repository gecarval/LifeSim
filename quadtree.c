/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadtree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:34:34 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/15 21:26:19 by anonymous        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

// POINTS HELPER FUNCTIONS
t_point	create_point(float_t x, float_t y, t_lifeform *life)
{
	return ((t_point){x, y, life});
}

float_t	sqdistfrompt(t_point *p, t_point *other)
{
	float_t	dx;
	float_t	dy;

	dx = other->x - p->x;
	dy = other->y - p->y;
	return (dx * dx + dy * dy);
}

float_t	distfrompt(t_point *p, t_point *other)
{
	return (sqrtf(sqdistfrompt(p, other)));
}

// RECTANGLE HELPERS FUNCTIONS
t_rectangle	create_rectangle(float_t x, float_t y, float_t w, float_t h)
{
	return ((t_rectangle){x, y, w, h, x - w / 2, x + w / 2, y - h / 2, y + h / 2});
}

int	quadcontains(t_rectangle *r, t_point *p)
{
	return (r->left <= p->x && p->x <= r->right && r->top <= p->y && p->y <= r->bottom);
}

int	quadintersects(t_rectangle *r, t_rectangle *range)
{
	return (!(r->right < range->left || range->right < r->left || r->bottom < range->top || range->bottom < r->top));
}

t_rectangle	subdivide(t_rectangle *r, const char *quadrant)
{
	if (ft_strncmp(quadrant, "ne", 2) == 0)
		return (create_rectangle(r->x + r->w / 4, r->y - r->h / 4, r->w / 2, r->h / 2));
	else if (ft_strncmp(quadrant, "nw", 2) == 0)
		return (create_rectangle(r->x - r->w / 4, r->y - r->h / 4, r->w / 2, r->h / 2));
	else if (ft_strncmp(quadrant, "se", 2) == 0)
		return (create_rectangle(r->x + r->w / 4, r->y + r->h / 4, r->w / 2, r->h / 2));
	else if (ft_strncmp(quadrant, "sw", 2) == 0)
		return (create_rectangle(r->x - r->w / 4, r->y + r->h / 4, r->w / 2, r->h / 2));
	return (create_rectangle(0, 0, 0, 0));
}

float_t	xdistfromrec(t_rectangle *rect, t_point *point)
{
	if (rect->left <= point->x && point->x <= rect->right)
		return (0);
	return (fmin(fabs(point->x - rect->left), fabs(point->x - rect->right)));
}

float_t	ydistfromrec(t_rectangle *rect, t_point *point)
{
	if (rect->top <= point->y && point->y <= rect->bottom)
		return (0);
	return (fminf(fabsf(point->y - rect->top), fabsf(point->y - rect->bottom)));
}

float_t	sqdistfromrec(t_rectangle *rect, t_point *point)
{
	float_t	dx;
	float_t	dy;

	dx = xdistfromrec(rect, point);
	dy = ydistfromrec(rect, point);
	return (dx * dx + dy * dy);
}

float_t	distfrom(t_rectangle *rect, t_point *point)
{
	return (sqrtf(sqdistfromrec(rect, point)));
}

// CIRCLE HELPER FUNCTIONS
t_circle	create_circle(float_t x, float_t y, float_t r)
{
	return ((t_circle){x, y, r, r + r});
}

int	circontains(t_circle *c, t_point *p)
{
	return ((p->x - c->x) * (p->x - c->x) + (p->y - c->y) * (p->y - c->y) <= c->rsqrt);
}

int	cirintersects(t_circle *c, t_rectangle *range)
{
	float_t	xdist;
	float_t	ydist;
	float_t	w;
	float_t	h;
	float_t	edges;

	xdist = fabs(range->x - c->x);
	ydist = fabs(range->y - c->y);
	w = range->w / 2;
	h = range->h / 2;
	edges = (xdist - w) * (xdist - w) + (ydist - h) * (ydist - h);
	if (xdist > c->r + w || ydist > c->r + h)
		return (0);
	if (xdist <= w || ydist <= h)
		return (1);
	return (edges <= c->rsqrt);
}

// QUADTREE HELPER FUNCTIONS
t_quadtree	*create_quadtree(t_rectangle boundary, int capacity, int depth)
{
	t_quadtree	*qt;

	qt = (t_quadtree *)malloc(sizeof(t_quadtree));
	qt->boundary = boundary;
	qt->capacity = capacity;
	qt->depth = depth;
	qt->divided = false;
	qt->northeast = NULL;
	qt->northwest = NULL;
	qt->southeast = NULL;
	qt->southwest = NULL;
	qt->points = (t_point **)malloc(capacity * sizeof(t_point *));
	return (qt);
}

void	subdivide_tree(t_quadtree *qt)
{
	t_rectangle	ne;
	t_rectangle	nw;
	t_rectangle	se;
	t_rectangle	sw;

	ne = create_rectangle(qt->boundary.x + qt->boundary.w / 2, qt->boundary.y - qt->boundary.h / 2, qt->boundary.w / 2, qt->boundary.h / 2);
	qt->northeast = create_quadtree(ne, qt->capacity, qt->depth + 1);
	nw = create_rectangle(qt->boundary.x - qt->boundary.w / 2, qt->boundary.y - qt->boundary.h / 2, qt->boundary.w / 2, qt->boundary.h / 2);
	qt->northwest = create_quadtree(nw, qt->capacity, qt->depth + 1);
	se = create_rectangle(qt->boundary.x + qt->boundary.w / 2, qt->boundary.y + qt->boundary.h / 2, qt->boundary.w / 2, qt->boundary.h / 2);
	qt->southeast = create_quadtree(se, qt->capacity, qt->depth + 1);
	sw = create_rectangle(qt->boundary.x - qt->boundary.w / 2, qt->boundary.y + qt->boundary.h / 2, qt->boundary.w / 2, qt->boundary.h / 2);
	qt->southwest = create_quadtree(sw, qt->capacity, qt->depth + 1);
	qt->divided = true;
}

/*t_quadtree	*get_children(t_quadtree *qt, t_rectangle rect, const char *s)
{
	if (subdividerect(qt->, s))
		return ();
	else
		return ();
}*/
