#include "mlx.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <unistd.h>
#include <math.h>


#define mapWidth 24
#define mapHeight 24
#define screenWidth 1920
#define screenHeight 1080
#define texWidth 64
#define texHeight 64

char *texPath[8] = {
	"./textures/eagle.xpm",
	"./textures/pillar.xpm",
	"./textures/barrel.xpm",
	"./textures/greystone.xpm",
	"./textures/bluestone.xpm",
	"./textures/mossy.xpm",
	"./textures/greenlight.xpm",
	"./textures/colorstone.xpm"
};

typedef struct s_color {
  int r;
  int g;
  int b;
} t_color;

t_color RGB_Red = {255, 0, 0};
t_color RGB_Green = {0, 255, 0};
t_color RGB_Blue = {0, 0, 255};
t_color RGB_White = {255, 255, 255};
t_color RGB_Yellow = {255, 255, 0};

typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

typedef struct  s_vectors
{
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
}               t_vectors;


typedef struct  s_args
{
    void        *mlx_ptr;
    void        *win_ptr;
    t_data      *data;
    t_vectors   *vectors;
    void        **images;
}              t_args;


int worldMap[mapWidth][mapHeight]=
{
  {4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,7,7,7,7,7,7,7,7},
  {4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7},
  {4,0,3,0,0,0,0,0,0,0,0,0,0,0,0,0,7,0,0,0,0,0,0,7},
  {4,0,4,0,0,0,0,5,5,5,5,5,5,5,5,5,7,7,0,7,7,7,7,7},
  {4,0,5,0,0,0,0,5,0,5,0,5,0,5,0,5,7,0,0,0,7,7,7,1},
  {4,0,6,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,7,7,7,1},
  {4,0,8,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,0,0,0,8},
  {4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,5,7,0,0,0,7,7,7,1},
  {4,0,0,0,0,0,0,5,5,5,5,0,5,5,5,5,7,7,7,7,7,7,7,1},
  {6,6,6,6,6,6,6,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {8,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4},
  {6,6,6,6,6,6,0,6,6,6,6,0,6,6,6,6,6,6,6,6,6,6,6,6},
  {4,4,4,4,4,4,0,4,4,4,6,0,6,2,2,2,2,2,2,2,3,3,3,3},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,0,0,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,0,0,0,0,5,0,0,0,0,0,0,2},
  {4,0,0,5,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,2,0,2,2},
  {4,0,6,0,6,0,0,0,0,4,6,0,6,2,0,0,5,0,0,2,0,0,0,2},
  {4,0,0,0,0,0,0,0,0,4,6,0,6,2,0,0,0,0,0,2,0,0,0,2},
  {4,4,4,4,4,4,4,4,4,4,1,1,1,2,2,2,2,2,2,3,3,3,3,3}
};
double dabs(double num)
{
  if (num < 0)
    return (-num);
  return (num);
}
void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
int close_window(int keycode, void *arg)
{
    // Realizar cualquier limpieza o liberación de recursos necesarios
	keycode = keycode;
	arg = arg;
    exit(0);
}

void	verLine(int x, int drawStart, int drawEnd, t_color color, t_data *data)
{
  int y;
  for (y = drawStart; y < drawEnd; y++) {
    my_mlx_pixel_put(data, x, y, color.r << 16 | color.g << 8 | color.b);
  }
}

int deal_key(int keycode, void *param)
{
  t_args *args = param;
	t_vectors *par = args->vectors;
  void *mlx_ptr = args->mlx_ptr;
  void *win_ptr = args->win_ptr;
  t_data data = *args->data;
  void **images = args->images;

  double posX = par->posX;
  double posY = par->posY;
  double dirX = par->dirX;
  double dirY = par->dirY;
  double planeX = par->planeX;
  double planeY = par->planeY;
  double moveSpeed = 0.5;
  double rotSpeed = 0.25;

	if (keycode == 65307)
    	exit(0);
  if (keycode == 65361 || keycode == 65362 || keycode == 65363 || keycode == 65364)
  {
    if (keycode == 65362)
    {
      if(worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == 0) posX += dirX * moveSpeed;
      if(worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == 0) posY += dirY * moveSpeed;
    }
    //move backwards if no wall behind you
    if (keycode == 65364)
    {
      if(worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == 0) posX -= dirX * moveSpeed;
      if(worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == 0) posY -= dirY * moveSpeed;
    }
    //rotate to the right
    if (keycode == 65363)
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
      dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
      planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
    }
    //rotate to the left
    if (keycode == 65361)
    {
      //both camera direction and camera plane must be rotated
      double oldDirX = dirX;
      dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
      dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
      double oldPlaneX = planeX;
      planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
      planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
    }
  }
  int i, j;
  for (i = 0; i < screenWidth; i++)
  {
    for (j = 0; j < screenHeight; j++)
    {
      my_mlx_pixel_put(&data, i, j, 0);
    }
  }
  int x;
  for ( x = 0; x < screenWidth; x++)
	{
		//calculate ray position and direction
      double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
      double rayDirX = dirX + planeX*cameraX;
      double rayDirY = dirY + planeY*cameraX;

      //which box of the map we're in
      int mapX = (int)(posX);
      int mapY = (int)(posY);

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

	  //length of ray from one x or y-side to next x or y-side
      double deltaDistX = (rayDirX == 0) ? 1e30 : dabs((1.0 / rayDirX));
      double deltaDistY = (rayDirY == 0) ? 1e30 : dabs((1.0 / rayDirY));
      double perpWallDist;
      if (x == 800)
        printf("Raydirx: %f, Raydiry: %f\n", rayDirX, rayDirY);

      if (x == 800)
        printf("Deltadistx: %f, Deltadisty: %f\n", deltaDistX, deltaDistY);
      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?

	  //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }
      if (x == 800)
        printf("Initial sidedistX: %f, sideDistY: %f\n", sideDistX, sideDistY);
      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
      } 

      //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
      if(side == 0) perpWallDist = (sideDistX - deltaDistX);
      else          perpWallDist = (sideDistY - deltaDistY);
      if (x == 800)
      {
        printf("side: %d, sideDistX800: %f, sideWallDistY: %f, deltaDistX: %f, deltaDistY: %f\n", side, sideDistX, sideDistY, deltaDistX, deltaDistY);
      }

      //Calculate height of line to draw on screen
      int lineHeight = (int)(screenHeight / perpWallDist);
      if (x == 800)
      {
        printf("lineHeight800: %d\nPerpWallDist800: %f\n", lineHeight, perpWallDist);
      }
      if (x == 1000)
      {
        printf("lineHeight1000: %d\nPerpWallDist1000: %f\n", lineHeight, perpWallDist);
      }
      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + screenHeight / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + screenHeight / 2;
      if(drawEnd >= screenHeight)drawEnd = screenHeight - 1;
      if (x == 800)
      {
        printf("drawStart800: %d\n", drawStart);
        printf("drawEnd800: %d\n", drawEnd);
      }
      if (x == 1000)
      {
        printf("drawStart1000: %d\n", drawStart);
        printf("drawEnd10000: %d\n", drawEnd);
      }
      //texturing calculations
      int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!
      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if (side == 0) wallX = posY + perpWallDist * rayDirY;
      else           wallX = posX + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX = (int)(wallX * (double)(texWidth));
      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;
      
      // How much to increase the texture coordinate per screen pixel
      double step = 1.0 * texHeight / lineHeight;
      // Starting texture coordinate
      double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step;
      t_data imgTexture;
      char *dst;
      for(int y = drawStart; y<drawEnd; y++)
      {
        imgTexture.img = images[texNum];
        imgTexture.addr = mlx_get_data_addr(imgTexture.img, &imgTexture.bits_per_pixel, &imgTexture.line_length, &imgTexture.endian);
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;
        dst = imgTexture.addr + (texY * imgTexture.line_length + texX * (imgTexture.bits_per_pixel / 8));
        unsigned int color = *(unsigned int*)dst;
        if(side == 1) color = (color >> 1) & 8355711;
         //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        my_mlx_pixel_put(&data, x, y, color);
      }
	}
  par->posX = posX;
  par->posY = posY;
  par->dirX = dirX;
  par->dirY = dirY;
  par->planeX = planeX;
  par->planeY = planeY;

	mlx_put_image_to_window(mlx_ptr, win_ptr, data.img, 0, 0);
  printf("%d\n", keycode);
	return (0);
}


void	my_mlx_hooks(void *mlx_ptr, void *args) {
  t_args *arg = args;
  mlx_ptr = mlx_ptr;
  void *mlx_win = arg->win_ptr;
	mlx_hook(mlx_win, 17, 0, close_window, args);
	mlx_key_hook(mlx_win, deal_key, args);
	return ;
}

int main() {
	int x, i, img_w, img_h;
  void	*mlx_ptr;
	void	*win_ptr;
  char *dst;
  t_args args;
	t_data	data;

	// MLX initilizing.
	mlx_ptr = mlx_init();
	win_ptr = mlx_new_window(mlx_ptr, screenWidth, screenHeight, "mlx 42");
  args.mlx_ptr = mlx_ptr;
  args.win_ptr = win_ptr;
  args.data = &data;
	my_mlx_hooks(mlx_ptr, &args);
	data.img = mlx_new_image(mlx_ptr, screenWidth, screenHeight);
	data.addr = mlx_get_data_addr(data.img, &data.bits_per_pixel, &data.line_length, &data.endian);
	// Raycasting variables
	double posX = 22.0, posY = 11.5;  //x and y start position
	double dirX = -1.0, dirY = 0.0; //initial direction vector
	double planeX = 0.0, planeY = 0.393; //the 2d raycaster version of camera plane
  t_vectors vect;
  vect.posX = posX;
  vect.posY = posY;
  vect.dirX = dirX;
  vect.dirY = dirY;
  vect.planeX = planeX;
  vect.planeY = planeY;
  args.vectors = &vect;
	void	*images[8];
	for (i = 0; i < 8; i++)
		images[i] = mlx_xpm_file_to_image(mlx_ptr, texPath[i], &img_w, &img_h);
  
  args.images = images;

	for ( x = 0; x < screenWidth; x++)
	{
		//calculate ray position and direction
      double cameraX = 2 * x / (double)screenWidth - 1; //x-coordinate in camera space
      double rayDirX = dirX + planeX*cameraX;
      double rayDirY = dirY + planeY*cameraX;

      //which box of the map we're in
      int mapX = (int)(posX);
      int mapY = (int)(posY);

      //length of ray from current position to next x or y-side
      double sideDistX;
      double sideDistY;

	  //length of ray from one x or y-side to next x or y-side
      double deltaDistX = (rayDirX == 0) ? 1e30 : dabs((1.0 / rayDirX));
      double deltaDistY = (rayDirY == 0) ? 1e30 : dabs((1.0 / rayDirY));
      double perpWallDist;

      //what direction to step in x or y-direction (either +1 or -1)
      int stepX;
      int stepY;

      int hit = 0; //was there a wall hit?
      int side; //was a NS or a EW wall hit?

	  //calculate step and initial sideDist
      if (rayDirX < 0)
      {
        stepX = -1;
        sideDistX = (posX - mapX) * deltaDistX;
      }
      else
      {
        stepX = 1;
        sideDistX = (mapX + 1.0 - posX) * deltaDistX;
      }
      if (rayDirY < 0)
      {
        stepY = -1;
        sideDistY = (posY - mapY) * deltaDistY;
      }
      else
      {
        stepY = 1;
        sideDistY = (mapY + 1.0 - posY) * deltaDistY;
      }

      //perform DDA
      while (hit == 0)
      {
        //jump to next map square, either in x-direction, or in y-direction
        if (sideDistX < sideDistY)
        {
          sideDistX += deltaDistX;
          mapX += stepX;
          side = 0;
        }
        else
        {
          sideDistY += deltaDistY;
          mapY += stepY;
          side = 1;
        }
        //Check if ray has hit a wall
        if (worldMap[mapX][mapY] > 0) hit = 1;
      } 

      //Calculate distance projected on camera direction (Euclidean distance would give fisheye effect!)
      if(side == 0) perpWallDist = (sideDistX - deltaDistX);
      else          perpWallDist = (sideDistY - deltaDistY);

      //Calculate height of line to draw on screen
      int lineHeight = (int)(screenHeight / perpWallDist);

      //calculate lowest and highest pixel to fill in current stripe
      int drawStart = -lineHeight / 2 + screenHeight / 2;
      if(drawStart < 0)drawStart = 0;
      int drawEnd = lineHeight / 2 + screenHeight / 2;
      if(drawEnd >= screenHeight)drawEnd = screenHeight - 1;

      //texturing calculations
      int texNum = worldMap[mapX][mapY] - 1; //1 subtracted from it so that texture 0 can be used!
      //calculate value of wallX
      double wallX; //where exactly the wall was hit
      if (side == 0) wallX = posY + perpWallDist * rayDirY;
      else           wallX = posX + perpWallDist * rayDirX;
      wallX -= floor((wallX));

      //x coordinate on the texture
      int texX = (int)(wallX * (double)(texWidth));
      if(side == 0 && rayDirX > 0) texX = texWidth - texX - 1;
      if(side == 1 && rayDirY < 0) texX = texWidth - texX - 1;
      
      // How much to increase the texture coordinate per screen pixel
      double step = 1.0 * texHeight / lineHeight;
      // Starting texture coordinate
      double texPos = (drawStart - screenHeight / 2 + lineHeight / 2) * step;
      t_data imgTexture;
      for(int y = drawStart; y<drawEnd; y++)
      {
        imgTexture.img = images[texNum];
        imgTexture.addr = mlx_get_data_addr(imgTexture.img, &imgTexture.bits_per_pixel, &imgTexture.line_length, &imgTexture.endian);
        // Cast the texture coordinate to integer, and mask with (texHeight - 1) in case of overflow
        int texY = (int)texPos & (texHeight - 1);
        texPos += step;
        dst = imgTexture.addr + (texY * imgTexture.line_length + texX * (imgTexture.bits_per_pixel / 8));
        unsigned int color = *(unsigned int*)dst;
         //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        if(side == 1) color = (color >> 1) & 8355711;
        //make color darker for y-sides: R, G and B byte each divided through two with a "shift" and an "and"
        my_mlx_pixel_put(&data, x, y, color);
      }

	}
	mlx_put_image_to_window(mlx_ptr, win_ptr, data.img, 0, 0);

	//start of the loop
	mlx_loop(mlx_ptr);
	return (0);

}