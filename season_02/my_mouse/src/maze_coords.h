#ifndef MAZE_COORDS_H
#define MAZE_COORDS_H

typedef unsigned int uint;

typedef struct maze_coords MazeCoords;
struct maze_coords {
    uint row;
    uint col;
    void (*delete)(MazeCoords** self);
};

extern const struct maze_coords_class {
    MazeCoords* (*new)(uint row, uint col);
} MazeCoordsClass;

#endif
