#ifndef MAZE_PARSER_H
#define MAZE_PARSER_H

#include "maze.h"

extern const struct maze_parser {
    Maze* (*fromPath)(const char* path);
} MazeParser;

#endif
