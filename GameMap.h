#ifndef GAMEMAP_H
#define GAMEMAP_H

#define CELL_SIZE 20
#define MAP_WIDTH 28
#define MAP_HEIGHT 31

extern int pacman_map[MAP_HEIGHT][MAP_WIDTH]; 

class GameMap{
public:
    GameMap();
    ~GameMap();
    
    void resetMap();
    void removePebble(int r, int c);
    
    int pacManMap[MAP_HEIGHT][MAP_WIDTH];
};

#endif
