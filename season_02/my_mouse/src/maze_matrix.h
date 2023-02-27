#ifndef MAZE_MATRIX_H
#define MAZE_MATRIX_H

#include "maze_coords.h"

typedef unsigned int uint;
typedef struct maze_matrix MazeMatrix;
struct maze_matrix {
    uint num_rows;
    uint num_cols;
    char** rows;

    void (*setElement)(MazeMatrix* self, MazeCoords* coords, char value);
    void (*setRow)(MazeMatrix* self, uint index, char* content);
    void (*print)(MazeMatrix* self);
    void (*delete)(MazeMatrix** self);
};

extern const struct maze_matrix_class {
    MazeMatrix* (*new)(uint num_rows, uint num_cols);
} MazeMatrixClass;

#endif
