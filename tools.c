/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 17:38:51 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/13 18:44:46 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

// HOOKS
int	mouse_released(int key, t_data *data)
{
	if (key == 1)
		data->click_hold = 0;
	ft_printf("%d\n", key);
	return (0);
}

int	mouse_click(int key, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	if (key == 1)
		data->click_hold = 1;
	if (key == 3)
		data->click_hold = 0;
	return (0);
}

int	mlx_anim(t_data *data)
{
	int	i;

	if (data->anilsim == 1)
	{
		water_mark(data);
		life_sim(data);
		mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
		i = 0;
		while (i < data->timing)
			i++;
	}
	return (0);
}

int	mlx_cooked(int key, t_data *data)
{
	if (key == '1')
	{
		data->anilsim *= -1;
	}
	if (key == ESC)
		exit_data(data, 0);
	if (key == ']')
		data->timing += 10000000;
	if (key == 'r')
	{
		reset_rules(data->lsim->rules, 0.3, 1.0, 1);
		reset_rules(data->lsim->atrrules, 25 * RADIUS, 80 * RADIUS, -1);
		reset_rules(data->lsim->reprules, 10 * RADIUS, 17 * RADIUS, -1);
	}
	if (key == '[')
		if (data->timing > 9999999)
			data->timing -= 10000000;
	if (key == ' ')
	{
		render_background(data, 0x000000);
		mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
		water_mark(data);
	}
	if (key == 65362)
		data->anispeed += 0.0025;
	if (key == 65364)
		data->anispeed -= 0.0025;
	return (0);
}

// GRAPH
void	pixel_to_img(int x, int y, t_data *data, int color)
{
	char	*pixel;

	if ((x < 0 || x >= WINDX) || (y < 0 || y >= WINDY))
		return ;
	pixel = data->img->img_px + y * data->img->llen + x * (data->img->bpp / 8);
	*(int *)pixel = color;
}

void	pixel_to_img_float(float_t x, float_t y, t_data *data, int color)
{
	char	*pixel;

	if ((x < 0 || x >= WINDX) || (y < 0 || y >= WINDY))
		return ;
	pixel = data->img->img_px + (int)y * data->img->llen + (int)x * (data->img->bpp / 8);
	*(int *)pixel = color;
}

void	water_mark(t_data *data)
{
	mlx_string_put(data->ini, data->win,
		15, 15, 120000, "renders by gecarval");
	mlx_string_put(data->ini, data->win,
		15, 60, 120000, "Press 1 for Life Sim");
}

void	render_background(t_data *data, int color)
{
	int	x;
	int	y;

	x = -1;
	while (++x < WINDX)
	{
		y = -1;
		while (++y < WINDY)
			pixel_to_img(x, y, data, color);
	}
}

// GET DIM
int	get_x_size(const char *s)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (s[i] != '\0')
	{
		if (s[i] != '\0' && ft_isdigit(s[i]))
		{
			x++;
			while (s[i + 1] != '\0' && s[i] != ' ')
				i++;
		}
		i++;
	}
	return (x);
}

int	get_y_size(const char *s)
{
	int	i;
	int	y;

	i = -1;
	y = 1;
	if (!s)
		return (0);
	while (s[++i] != '\0')
		if (s[i + 1] != '\0' && s[i] == '\n')
			y++;
	return (y);
}

// FREE
void	ft_free_matrix(void **vec)
{
	int	i;

	i = -1;
	while (vec[++i])
		free(vec[i]);
	free(vec);
}

void	ft_free_tensor(void ***z)
{
	int	i;

	i = -1;
	while (z[++i])
	{
		ft_free_matrix(z[i]);
	}
	free(z);
}

// DELTAS
void	defdel(t_delta *a, float_t ini, float_t fin)
{
	a->ini = ini;
	a->fin = fin;
}

t_delta	revdel(t_delta a)
{
	int	tmp;

	tmp = a.ini;
	a.ini = a.fin;
	a.fin = tmp;
	return (a);
}

// OTHERS
void	print_matrix(char **vec)
{
	int	i;
	int	j;

	i = -1;
	while (vec[++i])
	{
		j = -1;
		while(vec[i][++j])
			ft_printf("%c|", vec[i][j]);
		ft_printf("\n");
	}
}
