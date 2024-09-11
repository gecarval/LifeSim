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

# define RED	0xFF0000
# define GREEN	0x00FF00
# define BLUE	0x0000FF
# define YELLOW	0xDD5555

// LIFE SIM
#define NUMBER_OF_LIFEFORM  (int)10

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
// PROCESS CALLS
void	ft_free_tensor(char ***z);
void	ft_free_matrix(char **vec);
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
