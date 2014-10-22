#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include "include.h"

Map_T *gUnusedMapList;

Map_T *LoadMap(char *filename)
{
	int c, x, y;
	Map_T *map = BlankMap();
	FILE *file;
	char line[128];

	file = fopen(filename, "r");
	c = fgetc(file);

	while(c != EOF){
		//Check for a heading.
		if(c == '#')
		{
			if(fgets(line, sizeof(line), file) != NULL)
			{
				switch(toupper(line[0]))
				{
				case 'T': if(!stricmp(line, "tiles"))
						  {
								for(y = 0;y < MAX_MAP_HEIGHT;y++)
									for(x = 0;x < MAX_MAP_WIDTH;x++)
										if((c = fgetc(file)) != EOF)
										{
											map->tiles[x][y].num = c;
										}else{
											printf("LoadMap(): Error reading in file, not the correct number of tiles\n");
										}
						  }
						  break;
				}
			}
		}
	}
	return map;
}

Map_T *BlankMap()
{
	int x, y;
	Map_T *map = NewMap();
	for(y = 0;y < MAX_MAP_HEIGHT;y++)
		for(x = 0;x < MAX_MAP_WIDTH;x++){
			map->tiles[x][y].num = 0;
			map->tiles[x][y].entities = NULL;
		}
	map->start.x = 0;
	map->start.y = 0;
	return map;
}

Map_T *StartMap()
{
	int x, y;
	Map_T *map = BlankMap();
	map->start.x = 1;
	map->start.y = 1;
	for(x = 0;x < 5;x++)
		for(y = 0;y < 5;y++)
			map->tiles[x][y].num = 1;
	return map;
}