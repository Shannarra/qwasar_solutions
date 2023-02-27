#ifndef MAZE_H
#define MAZE_H

#include "maze_coords.h"
#include <stdbool.h>

typedef struct maze Maze;
struct maze {
    struct maze_internals* _internals;
    bool valid;
    MazeCoords* entrance;
    MazeCoords* exit;
    bool (*isCorridorOrExit)(Maze* self, MazeCoords* coords);
    void (*walk)(Maze* self, MazeCoords* coords);
    void (*walkBack)(Maze* self, MazeCoords* coords);
    void (*print)(Maze* self);
    void (*delete)(Maze** self);
};

extern const struct maze_class {
    Maze* (*fromPath)(const char* path);
} MazeClass;

#endif
