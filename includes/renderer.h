/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:05:06 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/02 14:34:58 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

// LIBRARIES
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <time.h>
# include <math.h>
# include <aio.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"

// WINDOW MACROS
# define WINDX	(int)1600
# define WINDY	(int)900
# define ESC	65307 

# define RED	0xCC0000
# define GREEN	0x00CC00
# define BLUE	0x0000CC
# define YELLOW	0xCCCC00

// LIFE SIM
#define NUMBER_OF_LIFEFORM  (int)2000

// STRUCTS
typedef struct s_objinf
{
	float_t	pox;
	float_t	poy;
	float_t	sx;
	float_t	sy;
	float_t	color;
}	t_objinf;

typedef struct s_delta
{
	float_t	ini;
	float_t	fin;
}	t_delta;

typedef struct s_img
{
	void	*img_ptr;
	char	*img_px;
	int		end;
	int		llen;
	int		bpp;
}	t_img;

typedef struct s_vector
{
	float_t	x;
	float_t	y;
}	t_vector;

typedef struct s_lifeform
{
	t_vector	ppos;
	t_vector	pos;
	t_vector	vel;
	t_vector	acel;
	int		id;
	int		color;
	int		mass;
}	t_lifeform;

typedef struct s_lifesim
{
	t_lifeform	*life;
	float_t		g;
	float_t		**rules;
}	t_lifesim;

typedef struct s_data
{
	void		*ini;
	void		*win;
	t_img		*img;
	t_lifesim	*lsim;
	float_t		anispeed;
	int		anilsim;
	int		click_hold;
	int		mposx;
	int		mposy;
	int		timing;
}	t_data;

typedef struct s_circle
{
    float_t	x;
    float_t	y;
    float_t	r;
    float_t	rsqrt;
} t_circle;

typedef struct s_rectangle
{
	float_t	x;
	float_t	y;
	float_t	w;
	float_t	h;
	float_t	left;
	float_t	right;
	float_t	top;
	float_t	bottom;
}	t_rectangle;

typedef struct s_point
{
	float_t		x;
	float_t		y;
	t_lifeform	*life;
} t_point;

typedef struct s_quadtree
{
	t_rectangle		boundary;
	t_point			**points;
	int			capacity;
	int			pointcount;
	int			divided;
	struct s_quadtree	*northeast;
	struct s_quadtree	*northwest;
	struct s_quadtree	*southeast;
	struct s_quadtree	*southwest;
}	t_quadtree;

// WATER MARKS
void	water_mark(t_data *data);
void	controls_mark(t_data *data);
//PIXEL PUTS
void	pixel_to_img(int x, int y, t_data *data, int color);
void	pixel_to_img_float(float_t x, float_t y, t_data *data, int color);
void	render_background(t_data *data, int color);
void	draw_line_y(t_delta x, t_delta y, t_data *data, int color);
void	draw_line_x(t_delta x, t_delta y, t_data *data, int color);
void	draw_line(t_delta x, t_delta y, t_data *data, int color);
void	drawcircle(int xc, int yc, int x, int y, t_data *data, int color);
void	circlebres(int xc, int yc, int r, t_data *data, int color);
// MAP READ
void	create_lsim(t_data *data);
// LIFE SIM
void	life_sim(t_data *data);
// DELTAS
void	defdel(t_delta *a, float_t ini, float_t fin);
t_delta	revdel(t_delta a);
// VECTORS
t_vector	create_vector(float_t x, float_t y);
t_vector	vector_sub(t_vector v1, t_vector v2);
t_vector	vectordiv(t_vector v, float_t scalar);
t_vector	vector_setmag(t_vector v, float_t magnitude);
float_t		vector_magsq(t_vector v);
float_t		constrain(float_t val, float_t min, float_t max);
void		vectoradd(t_vector *v1, t_vector v2);
// PROCESS CALLS
void	ft_free_tensor(void ***z);
void	ft_free_matrix(void **vec);
void	display_error(t_data *data, char *msg);
void	ft_init_program(t_data *data);
void	create_tensor(t_data *data);
int		exit_data(t_data *data, int fd);
// HOOKS
int		mlx_cooked(int	key, t_data *data);
int		mlx_anim(t_data *data);
int		mouse_click(int key, int x, int y, t_data *data);
int		mouse_released(int key, t_data *data);

#endif
