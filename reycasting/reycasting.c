/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maneddam <marvin@42>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/11 14:35:47 by maneddam          #+#    #+#             */
/*   Updated: 2023/08/16 19:33:01 by maneddam         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

// int encode_rgb(int r, int g, int b) {
//     return ((r << 16) | (g << 8) | b);
// }

void	my_mlx_pixel_put(t_mlx_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

void	drawSpace(t_mlx_data *m, int x, int y)
{
	int	k;
	int	s;

	k = 0;
	while (k < 60)
	{
		s = 0;
		while (s < 60)
		{
			if (s == 0 || k == 0)
				my_mlx_pixel_put(m , s + (60 * x), k + (60 * y) * 0.001, 0x000000);
			else
				my_mlx_pixel_put(m , s + (60 * x), k + (60 * y), SPACE);
			s++;
		}
		k++;
	}

}

void	drawSquare(t_mlx_data *m, int x, int y)
{
	int	k;
	int	s;

	k = 0;
	while (k < 60)
	{
		s = 0;
		while (s < 60)
		{
			if (s == 0 || k == 0)
				my_mlx_pixel_put(m , s + (60 * x), k + (60 * y), 0x000000);
			else
				my_mlx_pixel_put(m , s + (60 * x), k + (60 * y), WALL);
			s++;
		}
		k++;
	}

}

void	drawPlayer(t_mlx_data *m, int x, int y)
{
	int	k;
	int	s;
	(void)x;
	(void)y;
	// int	color;
	// int middleX;
	// int middleY;
	// color = encode_rgb(0, 255, 128);
	// middleX = m->inf->max_len * 60/2;
	// middleY = m->inf->y_len * 60/2;
	printf("stepx %f\nstepy : %f\n", m->inf->p.stepMoveX, m->inf->p.stepMoveY);
	k = 0;
	while (k < m->inf->y_len * 60)
	{
		s = 0;
		if (k == m->inf->p.stepMoveY - 1 || k == m->inf->p.stepMoveY || k == m->inf->p.stepMoveY + 1)
		{
			while (s < m->inf->max_len * 60)
			{
				if (s == m->inf->p.stepMoveX - 1 || s == m->inf->p.stepMoveX  || s == m->inf->p.stepMoveX + 1)
				{
					my_mlx_pixel_put(m, s , k, 0xFFFFFF);
				}
				s++;
			}
		}
		k++;
	}

	// my_mlx_pixel_put(m,  x * 60, y* 60, 0xFFFFFF);
	// my_mlx_pixel_put(m,  x, y + 1, 0xFFFFFF);
	// my_mlx_pixel_put(m,  x + 3, y + 3, 0xFFFFFF);
	// my_mlx_pixel_put(m,  x + 4, y + 4, 0xFFFFFF);



}

void	displayMap(t_mlx_data *m, t_infos *inf)
{

	char	**map2d;
	int		i;
	int		j;

	i = 0;
	map2d = inf->map2d;
	mlx_destroy_image(m->mlx_ptr, m->img_ptr);
	m->img_ptr = mlx_new_image(m->mlx_ptr, inf->max_len * 60, inf->y_len * 60);
	m->addr = mlx_get_data_addr(m->img_ptr, &m->bits_per_pixel, &m->line_length, &m->endian);
	m->inf = inf;

	while (map2d[i])
	{
		j = 0;
		while (map2d[i][j])
		{
			if (map2d[i][j] == '1' || map2d[i][j] == 32)
				drawSquare(m, j, i);
			else
				drawSpace(m, j, i);
			// if (map2d[i][j] == '0')
			// 	drawSquare(m, i, j, 'S');
			j++;
		}
		i++;
	}
	drawPlayer(m, inf->p.x * 60, inf->p.y * 60);
	// draw_rays(m);
	mlx_put_image_to_window(m->mlx_ptr, m->wind_ptr, m->img_ptr, 0, 0);
}

bool hasWallat(t_mlx_data *m, double x, double y)
{
	int gridX;
	int gridY;
	if (x < 0 || x > m->inf->max_len * 60
	|| y < 0 || y > m->inf->y_len * 60) {
			return true;
	}
	// printf("y-> %f\n", y);
	// printf("x-> %f\n", x);

	gridX = floor(x / 60);
	gridY = floor(y / 60);
	return m->inf->map2d[gridY][gridX] == '1';
}

int	move(int keycode, t_mlx_data *m)
{
	
	if (keycode == ESC)
	{
		printf("You closed the game using ESC key !");
		mlx_destroy_window(m->mlx_ptr, m->wind_ptr);
		exit(0);
	}
	if (keycode == W)
	{
		if (hasWallat(m, m->inf->p.stepMoveX, m->inf->p.stepMoveY - 4))
		{
			printf("WALL\n");
			return 0;
		}
		m->inf->p.stepMoveY -= SPEED;
	}
	if (keycode == S)
	{
		if (hasWallat(m, m->inf->p.stepMoveX, m->inf->p.stepMoveY + 3))
		{
			printf("WALL\n");
			return 0;
		}
		m->inf->p.stepMoveY += SPEED;
	}
	if (keycode == D)
	{
		if (hasWallat(m, m->inf->p.stepMoveX + 3, m->inf->p.stepMoveY))
		{
			printf("WALL\n");
			return 0;
		}
		m->inf->p.stepMoveX += SPEED;
	}
	if (keycode == A)
	{
		if (hasWallat(m, m->inf->p.stepMoveX - 4, m->inf->p.stepMoveY))
		{
			printf("WALL\n");
			return 0;
		}
		m->inf->p.stepMoveX -= SPEED;
	}

	displayMap(m, m->inf);

	
	// drawPlayer(m, m->inf->p.y , m->inf->p.x );
	return 0;
}

void draw_ori_ray(t_mlx_data *mlx, int i, int j)
{
	int s;
	int k;

	// printf("i : %d\nj : %d\n", i, j);
	// printf("map2d[i] : %c\n", mlx->inf->map2d[i][]);
	
	while(mlx->inf->map2d[i])
	{
		k = 0;
		// if(i )
		while (k < 60)
		{
			s = 0;
			while (s < 60)
			{
				if (s == 30)
				{
					my_mlx_pixel_put(mlx, k + (j), s + (i), 0x000);
				}
				s++;
			}
			k++;
		}



		// my_mlx_pixel_put(mlx, 30 + (i), 30 + (j), 0xFFFFFF);
		i++;
	}
}

double get_decimal_number(double a)  
{
	double b;

	b = (int)a;
	return (a - b);
}

void draw_rays(t_mlx_data *mlx)
{
	int i;
	int j;
	int k;
	int s;

	i = (int)mlx->inf->p.y_tmp;
	j = (int)mlx->inf->p.x_tmp;
	double d = get_decimal_number(mlx->inf->p.x_tmp);
	double r = get_decimal_number(mlx->inf->p.y_tmp);
	printf("x :: %f\ny :: %f\n", mlx->inf->p.x_tmp, mlx->inf->p.y_tmp);
		k = 0;
		while (k < 60)
		{
			s = 0;
			if(k >= 60 * d)
			{
				while (s < 60)
				{
					if (s == 60 * r)
					{
						my_mlx_pixel_put(mlx, k + (j * 60), s + (i * 60), 0xFF0000);
					}
					s++;
				}
			}
			k++;
		}
}

void	reycasting(t_infos *inf)
{
	t_mlx_data	mlx;

	inf->p = player_pos(inf->map2d);



	mlx.inf = inf;
	mlx.map2d = inf->map2d;

	mlx.mlx_ptr = mlx_init();

	printf("x) %f\ny) %f\n", inf->p.x, inf->p.y);

	mlx.wind_ptr = mlx_new_window(mlx.mlx_ptr, inf->max_len * 60, inf->y_len * 60, "cub3d");
	mlx.img_ptr = mlx_new_image(mlx.mlx_ptr, inf->max_len * 60, inf->y_len * 60);
	mlx.addr = mlx_get_data_addr(mlx.img_ptr, &mlx.bits_per_pixel, &mlx.line_length, &mlx.endian);

	mlx.inf->p.x_tmp = mlx.inf->p.x + 0.5;
	mlx.inf->p.y_tmp = mlx.inf->p.y + 0.5;
	mlx.inf->p.stepMoveX = mlx.inf->p.x * 60;
	mlx.inf->p.stepMoveY = mlx.inf->p.y * 60;
	displayMap(&mlx, inf);

	mlx_hook(mlx.wind_ptr, 2, 3, move, &mlx);
	mlx_loop(mlx.mlx_ptr);

}
