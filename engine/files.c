/******************************************************************************/
/******************************************************************************/
#include "include.h"
#include <stdio.h>
#include <stdlib.h>
/******************************************************************************/
Map_T *gUnusedMapList;
/******************************************************************************/
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
/******************************************************************************/
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
	map->space = cpSpaceNew();
	map->space->gravity = cpv(0, -100);

	return map;
}
/******************************************************************************/
Map_T *StartMap()
{
	Entity_T *e;
	int x, y;
	Vec2i s;
	Vec2f p;
	Map_T *map = BlankMap();
	
	
	GLuint tiles[] =
	{ 
	  38, 39, 39, 39, 40,
	  54, 55, 55, 56, 56,
	  54, 55, 55, 56, 56,
	  54, 55, 55, 56, 56,
	  70, 71, 71, 71, 72
	};
	e = SetupMonster(0);


	for(x = 0;x < 5;x++)
		for(y = 0;y < 5;y++)
			map->tiles[x][y].num = tiles[x+y*5];
	s.x = 32;
	s.y = 32;
	map->tilesheet = CreateBasicSprite("tiles.png", s);
	e = SetupMonster(0);
	map->start.x = 2;
	map->start.y = 2;
	p.x = 3;
	p.y = 3;
	MoveEntity(e, map, PosAtTile(p));
	e->Think = &(MonsterHunt);
	e->nextThink = game->currentTime + 1000;
	return map;
}
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/