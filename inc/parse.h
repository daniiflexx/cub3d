typedef enum s_text {
    NO,
    SO,
    WE,
    EA,
}				t_text;

typedef struct s_color
{
	int		r;
	int		g;
	int		b;
}	t_color;

typedef struct s_prog {
    char *routes[4];
    t_color floorCol;
    t_color ceilCol;
}               t_prog;


t_color	ft_color_atoi(char *str);