/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quadtree.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anonymous <anonymous@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:34:34 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/16 20:48:18 by gecarval         ###   ########.fr       */
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

t_rectangle	subdivide_rectangle(t_rectangle *r, const char *quadrant)
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

float_t	distfromrec(t_rectangle *rect, t_point *point)
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
t_quadtree	*create_quadtree(t_rectangle boundary, unsigned int capacity, unsigned int depth)
{
	t_quadtree	*qt;

	qt = (t_quadtree *)malloc(sizeof(t_quadtree));
	if (!qt)
		return (NULL);
	qt->points = (t_point *)malloc(capacity * sizeof(t_point));
	if (!qt->points)
		free(qt);
	if (!qt->points)
		return (NULL);
	qt->boundary = boundary;
	qt->capacity = capacity;
	qt->depth = depth;
	qt->divided = false;
	qt->point_count = 0;
	qt->northeast = NULL;
	qt->northwest = NULL;
	qt->southeast = NULL;
	qt->southwest = NULL;
	return (qt);
}

t_quadtree	*create_quadtree_fromglobals(float_t width, float_t height)
{
	t_rectangle	bounds;

	if (width == 0 || height == 0)
		return (NULL);
	bounds = create_rectangle(width / 2, height / 2, width, height);
	return (create_quadtree(bounds, DEFAULT_CAPACITY, 0));
}

t_quadtree	*create_quadtree_fromrectangle(t_rectangle rect, int capacity)
{
	if (capacity <= 0)
		capacity = DEFAULT_CAPACITY;
	return (create_quadtree(rect, capacity, 0));
}

t_quadtree	*create_quadtree_fromvalues(float_t x, float_t y, float_t w, float_t h, int capacity)
{
	t_rectangle	rect;

	if (capacity <= 0)
		capacity = DEFAULT_CAPACITY;
	rect = create_rectangle(x, y, w, h);
	return (create_quadtree(rect, capacity, 0));
}

t_quadtree	**getchildren(t_quadtree *qt)
{
	t_quadtree	**children;
	int		i;

	children = (t_quadtree **)malloc(4 * sizeof(t_quadtree *));
	if (qt->divided)
	{
		children[0] = qt->northeast;
		children[1] = qt->northwest;
		children[2] = qt->southeast;
		children[3] = qt->southwest;
	}
	else
	{
		i = -1;
		while (++i < 4)
			children[i] = NULL;
	}
	return (children);
}

t_quadtree	*get_one_children(t_quadtree *qt, const char *quadrant)
{
	if (qt->divided == 1)
	{
		if (ft_strncmp(quadrant, "ne", 2) == 0)
			return (qt->northeast);
		else if (ft_strncmp(quadrant, "nw", 2) == 0)
			return (qt->northwest);
		else if (ft_strncmp(quadrant, "se", 2) == 0)
			return (qt->southeast);
		else if (ft_strncmp(quadrant, "sw", 2) == 0)
			return (qt->southwest);
	}
	return (NULL);
}

void	subdivide_tree(t_quadtree *qt)
{
	int	i;
	bool	inserted;
	t_point	p;

	qt->northeast = create_quadtree(subdivide_rectangle(&qt->boundary, "ne"), qt->capacity, qt->depth + 1);
	qt->northwest = create_quadtree(subdivide_rectangle(&qt->boundary, "nw"), qt->capacity, qt->depth + 1);
	qt->southeast = create_quadtree(subdivide_rectangle(&qt->boundary, "se"), qt->capacity, qt->depth + 1);
	qt->southwest = create_quadtree(subdivide_rectangle(&qt->boundary, "sw"), qt->capacity, qt->depth + 1);
	qt->divided = true;
	i = -1;
	while (++i < qt->point_count)
	{
		p = qt->points[i];
		inserted = insert_point(qt->northeast, p) ||
			insert_point(qt->northwest, p) ||
			insert_point(qt->southeast, p) ||
			insert_point(qt->southwest, p);
		if (!inserted)
			exit(1);
	}
	free(qt->points);
	qt->points = NULL;
	qt->point_count = 0;
}

bool	insert_point(t_quadtree* qt, t_point p)
{
	if (!quadcontains(&qt->boundary, &p))
		return (0);
	if (qt->point_count < qt->capacity || qt->depth == MAX_DEPTH)
	{
		qt->points[qt->point_count++] = p;
		return (1);
	}
	if (!qt->divided)
		subdivide_tree(qt);
	return insert_point(qt->northeast, p) ||
		insert_point(qt->northwest, p) ||
		insert_point(qt->southeast, p) ||
		insert_point(qt->southwest, p);
}

void	query_quadtree(t_quadtree *qt, t_rectangle *range, t_point *foundpoints, int *foundcount)
{
	int	i;

	i = 0;
	if (!qt)
		return;
	if (!quadcontains(&qt->boundary, (t_point *)range))
		return;
	while (i < qt->point_count)
	{
		if (quadcontains(range, &qt->points[i]))
			foundpoints[(*foundcount)++] = qt->points[i];
		i++;
	}
	if (qt->divided)
	{
		query_quadtree(qt->northeast, range, foundpoints, foundcount);
		query_quadtree(qt->northwest, range, foundpoints, foundcount);
		query_quadtree(qt->southeast, range, foundpoints, foundcount);
		query_quadtree(qt->southwest, range, foundpoints, foundcount);
	}
}

void	print_quadtree(t_quadtree *qt)
{
	int	i;

	if (!qt)
		return;
	printf("QuadTree Boundary: x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", qt->boundary.x, qt->boundary.y, qt->boundary.w, qt->boundary.h);
	i = -1;
	while (++i < qt->point_count)
		printf("Point: (%.2f, %.2f)\n", qt->points[i].x, qt->points[i].y);
	if (qt->divided)
	{
		print_quadtree(qt->northeast);
		print_quadtree(qt->northwest);
		print_quadtree(qt->southeast);
		print_quadtree(qt->southwest);
	}
}
