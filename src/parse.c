#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "parse.h"
#include "get_next_line.h"

double	ft_atof(char *str)
{
	double	res;
	double	res2;
	char	*c;
	int		len;

	c = (char *)str;
	res = (double)atoi(c);
	while (*c && *c != '.')
		c++;
	if (*c == '.')
		c++;
	res2 = (double)atoi(c);
	len = strlen(c);
	while (len--)
		res2 /= 10;
	if (res >= 0)
		return (res + res2);
	else
		return (res + -res2);
}
t_color	ft_color_atoi(char *str)
{
	t_color	color;
	int		i;

	i = 0;
	color.r = ft_atof(str);
	while (str[i] && str[i] != ',')
		i++;
	color.g = ft_atof(str + ++i);
	while (str[i] && str[i] != ',')
		i++;
	color.b = ft_atof(str + ++i);
	return (color);
}

int	new_object(char *line, t_prog *program)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (line[i] && line[i] != ' ')
		i++;
	tmp = malloc(sizeof(char) * (i + 1));
	if (tmp == NULL)
		return (EXIT_FAILURE);
	j = 0;
	while (j < i)
	{
		tmp[j] = line[j];
		j++;
	}
	tmp[j] = '\0';
	if (!strcmp(tmp, "NO"))
		program->routes[NO] = strdup(line + i);
	else if (!strcmp(tmp, "SO"))
		program->routes[SO] = strdup(line + i);
	else if (!strcmp(tmp, "WE"))
		program->routes[WE] = strdup(line + i);
	else if (!strcmp(tmp, "EA"))
		program->routes[EA] = strdup(line + i);
	else if (!strcmp(tmp, "F"))
		program->floorCol = ft_color_atoi(line + i);
	else if (!strcmp(tmp, "C"))
		program->ceilCol = ft_color_atoi(line + i);
	else
	{
		free(tmp);
		return (EXIT_FAILURE);
	}
	free(tmp);
	return (EXIT_SUCCESS);
}

int	parse_file(char *filename, t_prog *program)	
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		return (EXIT_FAILURE);
	line = get_next_line(fd);
	while (line)
	{		
		if (line[0] != '\n')
		{
			if (new_object(line, program))
				return (EXIT_FAILURE);
		}
		free(line);
		line = get_next_line(fd);
	}
	free(line);
	close(fd);
	return (EXIT_SUCCESS);
}

int main(int argc, char **argv) {
	t_prog program;
	int i;

	i = 0;
	while (i < 4)
		program.routes[i++] = NULL;
	if (argc == 2)
	{
		if (parse_file(argv[1], &program))
			return (EXIT_FAILURE);
	}
	else
		return (EXIT_FAILURE);
	i = 0;
	while (i < 4)
	{
		printf("%s\n", program.routes[i]);
		i++;
	}
	printf("%d\n", program.floorCol.r);
	printf("%d\n", program.floorCol.g);
	printf("%d\n", program.floorCol.b);
	printf("%d\n", program.ceilCol.r);
	printf("%d\n", program.ceilCol.g);
	printf("%d\n", program.ceilCol.b);
	return (EXIT_SUCCESS);
}