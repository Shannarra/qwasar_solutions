#include "maze_coords.h"
#include <stdlib.h>

static MazeCoords* new(uint row, uint col);
const struct maze_coords_class MazeCoordsClass = {
    .new = &new
};

static void delete(MazeCoords** self);
MazeCoords* new(uint row, uint col)
{
    MazeCoords* self = malloc(sizeof (MazeCoords));
    self->row = row;
    self->col = col;

    self->delete = &delete;
    return self;
}

void delete(MazeCoords** self)
{
    free(*self); *self = NULL;
}
