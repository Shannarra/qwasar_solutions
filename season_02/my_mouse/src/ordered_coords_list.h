#ifndef ORDERED_COORDS_LIST_H
#define ORDERED_COORDS_LIST_H

#include "maze_coords.h"

typedef struct ordered_coords_list OrderedCoordsList;
struct ordered_coords_list {
    struct internals* _internals;
    void (*insert)(OrderedCoordsList* self, MazeCoords* coords, uint key);
    MazeCoords* (*next)(OrderedCoordsList* self);
    void (*delete)(OrderedCoordsList** self);
};

extern const struct ordered_coords_list_class {
    OrderedCoordsList* (*new)();
} OrderedCoordsListClass;

#endif
