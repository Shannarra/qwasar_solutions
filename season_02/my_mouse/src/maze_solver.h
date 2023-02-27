#ifndef MAZE_SOLVER_H
#define MAZE_SOLVER_H

#include "maze.h"

extern const struct maze_solver {
    void (*solve)(const char* path);
} MazeSolver;

#endif
