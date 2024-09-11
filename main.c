/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:57:08 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/11 13:30:15 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

int	main(void)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		display_error(data, "malloc error\n");
	ft_init_program(data);
	create_lsim(data);
	data->anilsim = -1;
	data->anispeed = 0.01;
	data->click_hold = 0;
	data->timing = 0;
	water_mark(data);
	mlx_mouse_hook(data->win, mouse_click, data);
	mlx_hook(data->win, DestroyNotify, 0, exit_data, data);
	mlx_key_hook(data->win, mlx_cooked, data);
	mlx_loop_hook(data->ini, mlx_anim, data);
	mlx_loop(data->ini);
	return (0);
}
