#include "maze.h"
#include "char_map.h"
#include "maze_matrix.h"
#include "maze_parser.h"
#include "maze_internals.h"
#include <stdio.h>
#include <stdlib.h>

#define INVALID_MAP_ERR "MAP ERROR"

static Maze* from_path(const char* path);
const struct maze_class MazeClass = {
    .fromPath = &from_path
};

static bool is_corridor_or_exit(Maze* self, MazeCoords* square);
static void walk(Maze* self, MazeCoords* coords);
static void walk_back(Maze* self, MazeCoords* coords);
static void print(Maze* self);
static void delete(Maze** self);
Maze* from_path(const char* path)
{
    Maze* self = MazeParser.fromPath(path);
    self->isCorridorOrExit = &is_corridor_or_exit;
    self->walk = &walk;
    self->walkBack = &walk_back;
    self->print = &print;
    self->delete = &delete;
    return self;
}

static bool is_corridor(Maze* self, MazeCoords* square);
static bool is_exit(Maze* self, MazeCoords* square);
bool is_corridor_or_exit(Maze* self, MazeCoords* square)
{
    return is_corridor(self, square) || is_exit(self, square);
}

static bool is_within(Maze* self, MazeCoords* square);
bool is_corridor(Maze* self, MazeCoords* square)
{
    struct maze_internals* internals = self->_internals;
    return is_within(self, square)
           && internals->matrix->rows[square->row][square->col] == internals->char_map->corridor;
}

bool is_exit(Maze* self, MazeCoords* square)
{
    struct maze_internals* internals = self->_internals;
    return is_within(self, square)
           && internals->matrix->rows[square->row][square->col] == internals->char_map->exit;
}

bool is_within(Maze* self, MazeCoords* square)
{
    MazeMatrix* matrix = self->_internals->matrix;
    return square->row < matrix->num_rows && square->col < matrix->num_cols;
}

void walk(Maze* self, MazeCoords* coords)
{
    MazeMatrix* matrix = self->_internals->matrix;
    if (is_corridor(self, coords)) {
        matrix->setElement(matrix, coords, self->_internals->char_map->path);
    }
}

void walk_back(Maze* self, MazeCoords* coords)
{
    MazeMatrix* matrix = self->_internals->matrix;
    matrix->setElement(matrix, coords, self->_internals->char_map->corridor);
}

static void print_error_message();
static void print_header(struct maze_internals* _internals);
void print(Maze* self)
{
    if (!self->valid) {
        print_error_message();
        return;
    }
    struct maze_internals* _internals = self->_internals;
    print_header(_internals);
    _internals->matrix->print(_internals->matrix);
}

void print_error_message()
{
    puts(INVALID_MAP_ERR);
}

void print_header(struct maze_internals* _internals)
{
    printf("%ux%u", _internals->matrix->num_rows, _internals->matrix->num_cols);
    _internals->char_map->print(_internals->char_map);
    puts("");
}

static void delete_internals(struct maze_internals* _internals);
void delete(Maze** self)
{
    if ((*self)->valid) {
        delete_internals((*self)->_internals);
        (*self)->entrance->delete(&(*self)->entrance);
        (*self)->exit->delete(&(*self)->exit);
    }
    free(*self); *self = NULL;
}

void delete_internals(struct maze_internals* _internals)
{
    _internals->char_map->delete(&_internals->char_map);
    _internals->matrix->delete(&_internals->matrix);
    free(_internals); _internals = NULL;
}
