#include "mlx.h"
#include <stdlib.h>
#include <string.h>

#define E_DESTROY_NOTIFY 17

const int  wx = 15, wy = 10;
const char map[wy][wx] = {
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '1'},
	{'1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1', '1'},
};

typedef struct s_img
{
	void *base;
	void *addr;
	int	  w;
	int	  h;
	int	  bpp;
	int	  size_line;
	int	  endian;
} t_img;

typedef struct s_mlx
{
	void *base;
	void *win;
	t_img img;

	int wx;
	int wy;
} t_mlx;

// init
void create_img(void *mlx, t_img *img, int width, int height)
{
	img->base = mlx_new_image(mlx, width, height);
	img->w = width;
	img->h = height;
	img->addr =
		mlx_get_data_addr(img->base, &img->bpp, &img->size_line, &img->endian);
}

int handle_exit_window(t_mlx *mlx)
{
	mlx_destroy_image(mlx->base, mlx->img.base);
	mlx_destroy_window(mlx->base, mlx->win);
	exit(0);
}

void texture_to_img(void *mlx, char *filename, t_img *img)
{
	img->base = mlx_xpm_file_to_image(mlx, filename, &img->w, &img->h);
	img->addr =
		mlx_get_data_addr(img->base, &img->bpp, &img->size_line, &img->endian);
}
// end init

int get_color(t_img *img, int x, int y)
{
	char *target;
	int	  color;
	int	  idx;

	idx = y * img->size_line + x * (img->bpp / 8);
	target = img->addr + idx;
	color = *(int *)target;
	return (color);
}

void put_pixel_to_img(int color, int x, int y, t_img *img)
{
	char *target;
	int	  idx;

	idx = y * img->size_line + x * (img->bpp / 8);
	target = img->addr + idx;
	*(unsigned int *)target = color;
}

void put_texture_to_img(t_img *texture, int x, int y, t_img *img)
{
	for (int ty = 0; ty < texture->h; ++ty)
	{
		for (int tx = 0; tx < texture->w; ++tx)
		{
			int color = get_color(texture, tx, ty);
			put_pixel_to_img(color, x + tx, y + ty, img);
		}
	}
}

int main()
{
	t_mlx mlx;
	t_img texture[2];

	memset(&mlx, 0, sizeof(t_mlx));
	memset(&texture, 0, sizeof(t_img) * 2);

	mlx.base = mlx_init();
	texture_to_img(mlx.base, "0.xpm", &texture[0]);
	texture_to_img(mlx.base, "1.xpm", &texture[1]);
	mlx.wx = wx * texture[0].w;
	mlx.wy = wy * texture[0].h;

	mlx.win = mlx_new_window(mlx.base, mlx.wx, mlx.wy, "test_mlx");
	create_img(mlx.base, &mlx.img, mlx.wx, mlx.wy);

#ifdef USE_IMG_BUF
	// draw no tmp img
	for (int y = 0; y < wy; ++y)
	{
		for (int x = 0; x < wx; ++x)
		{
			int i = map[y][x] - '0';

			put_texture_to_img(&texture[i], x * texture[i].w, y * texture[i].h,
							   &mlx.img);
		}
	}
	mlx_put_image_to_window(mlx.base, mlx.win, mlx.img.base, 0, 0);
#else
	// draw no tmp img
	for (int y = 0; y < wy; ++y)
	{
		for (int x = 0; x < wx; ++x)
		{
			int i = map[y][x] - '0';

			mlx_put_image_to_window(mlx.base, mlx.win, texture[i].base,
									x * texture[i].w, y * texture[i].h);
		}
	}
#endif

	mlx_hook(mlx.win, E_DESTROY_NOTIFY, 0, handle_exit_window, &mlx);
	mlx_loop(mlx.base);
	return (0);
}
