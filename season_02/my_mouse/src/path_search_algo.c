#include "path_search_algo.h"
#include "ordered_coords_list.h"
#include <stdlib.h>
#include <limits.h>

static PathSearchAlgo* new(Maze* maze);
const struct path_search_algo_class PathSearchAlgoClass = {
        .new = &new
};

struct internals {
    Maze* maze;
    MazeCoords* goal;
};

static void ida_star(PathSearchAlgo* self);
static void delete(PathSearchAlgo** self);
PathSearchAlgo* new(Maze* maze)
{
    PathSearchAlgo* self = malloc(sizeof (PathSearchAlgo));
    self->_internals = malloc(sizeof (struct internals));
    self->_internals->maze = maze;
    self->_internals->goal = maze->exit;
    self->found = false;

    self->run = &ida_star;
    self->delete = &delete;
    return self;
}

static uint min_remaining_steps(PathSearchAlgo* self, MazeCoords* coords);
static uint search(PathSearchAlgo* self, MazeCoords* coords, uint num_steps);
/**
 * This is an implementation of the IDA* search algorithm (https://en.wikipedia.org/wiki/Iterative_deepening_A*).
 */
void ida_star(PathSearchAlgo* self)
{
    uint t = min_remaining_steps(self, self->_internals->maze->entrance);
    while (!self->found && t != UINT_MAX) {
        self->num_steps = t;
        t = search(self, self->_internals->maze->entrance, 0);
    }
}

uint min_remaining_steps(PathSearchAlgo* self, MazeCoords* coords)
{
    MazeCoords* goal = self->_internals->goal;
    return abs((int) (goal->row - coords->row)) + abs((int) (goal->col - coords->col));
}

static bool is_goal(PathSearchAlgo* self, MazeCoords* coords);
static OrderedCoordsList* ordered_neighbors_of(PathSearchAlgo* self, MazeCoords* coords);
uint search(PathSearchAlgo* self, MazeCoords* coords, uint num_steps)
{
    uint f = num_steps + min_remaining_steps(self, coords);
    if (f > self->num_steps) return f;
    if (is_goal(self, coords)) {
        self->found = true;
        return f;
    }
    uint min = UINT_MAX, t;
    Maze* maze = self->_internals->maze;
    MazeCoords* next;
    OrderedCoordsList* orderedNeighbors = ordered_neighbors_of(self, coords);
    while ((next = orderedNeighbors->next(orderedNeighbors))) {
        maze->walk(maze, next);
        t = search(self, next, num_steps + 1);
        if (self->found) {
            next->delete(&next);
            orderedNeighbors->delete(&orderedNeighbors);
            return t;
        }
        if (t < min) min = t;
        maze->walkBack(maze, next);
        next->delete(&next);
    }
    orderedNeighbors->delete(&orderedNeighbors);
    return min;
}

bool is_goal(PathSearchAlgo* self, MazeCoords* coords)
{
    MazeCoords* goal = self->_internals->goal;
    return goal->row == coords->row && goal->col == coords->col;
}

OrderedCoordsList* ordered_neighbors_of(PathSearchAlgo* self, MazeCoords* coords)
{
    static signed char directions[][2] = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};
    static unsigned char num_directions = 4;
    OrderedCoordsList* list = OrderedCoordsListClass.new();
    Maze* maze = self->_internals->maze;
    uint row, col;
    for (unsigned char i = 0; i < num_directions; i++) {
        row = coords->row + directions[i][0];
        col = coords->col + directions[i][1];
        if (maze->isCorridorOrExit(maze, &(MazeCoords) {.row = row, .col = col})) {
            MazeCoords* insertedCoords = MazeCoordsClass.new(row, col);
            list->insert(list, insertedCoords, min_remaining_steps(self, insertedCoords));
        }
    }
    return list;
}

void delete(PathSearchAlgo** self)
{
    free((*self)->_internals);
    free(*self); *self = NULL;
}
