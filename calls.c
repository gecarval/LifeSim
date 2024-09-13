/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:32:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/13 15:57:27 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

int	exit_data(t_data *data, int fd)
{
	if (data != NULL && data->img != NULL && data->img->img_ptr != NULL)
		mlx_destroy_image(data->ini, data->img->img_ptr);
	if (data != NULL && data->img != NULL)
		free(data->img);
	if (data != NULL && data->lsim != NULL && data->lsim->atrrules != NULL)
		ft_free_matrix((void **)data->lsim->atrrules);
	if (data != NULL && data->lsim != NULL && data->lsim->reprules != NULL)
		ft_free_matrix((void **)data->lsim->reprules);
	if (data != NULL && data->lsim != NULL && data->lsim->rules != NULL)
		ft_free_matrix((void **)data->lsim->rules);
	if (data != NULL && data->lsim != NULL && data->lsim->life != NULL)
		free(data->lsim->life);
	if (data != NULL && data->lsim != NULL)
		free(data->lsim);
	if (data != NULL && data->win != NULL)
		mlx_clear_window(data->ini, data->win);
	if (data != NULL && data->win != NULL)
		mlx_destroy_window(data->ini, data->win);
	if (data != NULL && data->ini != NULL)
		mlx_destroy_display(data->ini);
	if (data != NULL && data->ini != NULL)
		free(data->ini);
	if (data != NULL)
		free(data);
	exit(fd);
	return (0);
}

void	display_error(t_data *data, char *msg)
{
	ft_putstr_fd(msg, 2);
	exit_data(data, 1);
}

void	ft_init_program(t_data *data)
{
	data->ini = mlx_init();
	if (!data->ini)
		display_error(data, "init error\n");
	data->win = mlx_new_window(data->ini, WINDX, WINDY, "render");
	if (!data->win)
		display_error(data, "window error\n");
	data->lsim = (t_lifesim *)malloc(sizeof(t_lifesim));
	if (!data->lsim)
		display_error(data, "lifesim malloc error\n");
	data->lsim->life = NULL;
	data->img = (t_img *)malloc(sizeof(t_img));
	if (!data->img)
		display_error(data, "img malloc error\n");
	data->img->img_ptr = mlx_new_image(data->ini, WINDX, WINDY);
	if (!data->img->img_ptr)
		display_error(data, "img ptr error\n");
	data->img->img_px = mlx_get_data_addr(data->img->img_ptr, &data->img->bpp,
			&data->img->llen, &data->img->end);
	if (!data->img->img_px)
		display_error(data, "img data error\n");
}
