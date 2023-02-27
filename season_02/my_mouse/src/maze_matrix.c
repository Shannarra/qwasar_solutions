#include "maze_matrix.h"
#include <stdlib.h>
#include <stdio.h>

static MazeMatrix* new();
const struct maze_matrix_class MazeMatrixClass = {
    .new = &new
};

static void set_element(MazeMatrix* self, MazeCoords* coords, char value);
static void set_row(MazeMatrix* self, uint index, char* content);
static void print(MazeMatrix* self);
static void delete(MazeMatrix** self);
MazeMatrix* new(uint num_rows, uint num_cols)
{
    MazeMatrix* self = malloc(sizeof (MazeMatrix));
    self->num_rows = num_rows;
    self->num_cols = num_cols;
    self->rows = malloc(self->num_rows * sizeof (char*));

    self->setElement = &set_element;
    self->setRow = &set_row;
    self->print = &print;
    self->delete = &delete;
    return self;
}

void set_element(MazeMatrix* self, MazeCoords* coords, char value)
{
    self->rows[coords->row][coords->col] = value;
}

void set_row(MazeMatrix* self, uint index, char* content)
{
    self->rows[index] = content;
}

void print(MazeMatrix* self)
{
    for (uint i = 0; i < self->num_rows; i++) {
        puts(self->rows[i]);
    }
}

void delete(MazeMatrix** self)
{
    for (uint i = 0; i < (*self)->num_rows; i++) {
        free((*self)->rows[i]);
    }
    free((*self)->rows);
    free(*self); *self = NULL;
}
