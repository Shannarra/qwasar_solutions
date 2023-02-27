#include "maze_parser.h"
#include "my_libc/readline.h"
#include "char_map.h"
#include "maze_matrix.h"
#include "maze_internals.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Maze* from_path(const char* path);
const struct maze_parser MazeParser = {
    .fromPath = &from_path
};

typedef struct parser {
    int fd;
    CharMap* char_map;
    MazeMatrix* matrix;
    MazeCoords* entrance;
    MazeCoords* exit;
    bool failed;
    void (*init)();
} Parser;

static void init();
static Parser parser = {
    .init = &init
};

void init()
{
    parser.entrance = parser.exit = NULL;
    parser.failed = false;
}

static void parse_header();
static void fill_matrix();
static Maze* new_maze();
Maze* from_path(const char* path)
{
    parser.init();
    if ((parser.fd = open(path, O_RDONLY)) == -1) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Can't open file");
    }
    if (!parser.failed) parse_header();
    if (!parser.failed) fill_matrix();
    if (parser.fd != -1) close(parser.fd);
    return new_maze();
}

static char* initialize_matrix(char* header);
static void initialize_char_map(char* char_map);
void parse_header()
{
    char *header, *char_map;
    if (!(header = readline(parser.fd))){
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "File is empty");
    }
    if (!parser.failed) char_map = initialize_matrix(header);
    if (!parser.failed) initialize_char_map(char_map);
    if (header) free(header);
}

char* initialize_matrix(char* header)
{
    long num_rows, num_cols;
    if ((num_rows = strtol(header, &header, 10)) <= 0) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Header specifies invalid number of rows");
    }
    if (!parser.failed
        && ((num_cols = strtol(header + 1, &header, 10)) <= 0
            || num_cols > 1000)) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Header specifies invalid number of columns (max 1,000)");
    }
    if (!parser.failed && num_rows * num_cols > 1000000000) /* one billion */ {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Header specifies a number of squares greater than one billion");
    }
    if (!parser.failed) parser.matrix = MazeMatrixClass.new(num_rows, num_cols);
    return header;
}

void initialize_char_map(char* char_map)
{
    parser.char_map = CharMapClass.new();
    if (strlen(char_map) != 5) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Header should specify a character mapping of length 5");
    }
    if (!parser.failed) {
        parser.char_map->wall = char_map[0];
        parser.char_map->corridor = char_map[1];
        parser.char_map->path = char_map[2];
        parser.char_map->entrance = char_map[3];
        parser.char_map->exit = char_map[4];
    }
    if (!parser.failed && !parser.char_map->hasDistinctElements(parser.char_map)) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Character mapping contains repeated elements");
    }
}

static void validate_row(char* row, uint row_index);
void fill_matrix()
{
    char* row;
    for (uint i = 0; !parser.failed && i < parser.matrix->num_rows; i++) {
        if (!(row = readline(parser.fd))) {
            parser.failed = true;
            dprintf(STDERR_FILENO, "Maze has fewer rows than specified in the header (%u)\n", parser.matrix->num_rows);
        }
        if (!parser.failed) validate_row(row, i);
        parser.matrix->setRow(parser.matrix, i, row);
    }
    if (!parser.failed && readline(parser.fd)) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "Maze has more rows than specified in the header (%u)\n", parser.matrix->num_rows);
    }
}

void validate_row(char* row, uint row_index)
{
    uint col_count = strlen(row);
    if (col_count != parser.matrix->num_cols) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "Row %u (0-based) has %u squares; should have %u\n", row_index, col_count, parser.matrix->num_cols);
    }
    for (uint j = 0; !parser.failed && j < col_count; j++) {
        if (row[j] == parser.char_map->entrance) {
            if (parser.entrance) {
                parser.failed = true;
                dprintf(STDERR_FILENO, "%s\n", "Maze has multiple entrances (exactly one required)");
            } else {
                parser.entrance = MazeCoordsClass.new(row_index, j);
            }
        } else if (row[j] == parser.char_map->exit) {
            if (parser.exit) {
                parser.failed = true;
                dprintf(STDERR_FILENO, "%s\n", "Maze has multiple exits (exactly one required)");
            } else {
                parser.exit = MazeCoordsClass.new(row_index, j);
            }
        } else if (row[j] != parser.char_map->wall
                    && row[j] != parser.char_map->corridor
                    && row[j] != parser.char_map->path) {
            parser.failed = true;
            dprintf(STDERR_FILENO, "Invalid character '%c' at row %u, column %u (0-based)\n", row[j], row_index, j);
        }
    }
}

static void verify_and_set_entrance_and_exit(Maze* maze);
static void delete_entrance_and_exit();
static void initialize_internals(Maze* maze);
Maze* new_maze()
{
    Maze* maze = malloc(sizeof (Maze));
    if (!parser.failed) verify_and_set_entrance_and_exit(maze);
    if (parser.failed) delete_entrance_and_exit();
    if (!parser.failed) initialize_internals(maze);
    maze->valid = !parser.failed;
    return maze;
}

void verify_and_set_entrance_and_exit(Maze* maze)
{
    if (!parser.entrance) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Maze has no entrance (exactly one required)");;
        return;
    }
    if (!parser.exit) {
        parser.failed = true;
        dprintf(STDERR_FILENO, "%s\n", "Maze has no exit (exactly one required)");
        return;
    }
    maze->entrance = parser.entrance;
    maze->exit = parser.exit;
}

void delete_entrance_and_exit()
{
    if (parser.entrance) parser.entrance->delete(&parser.entrance);
    if (parser.exit) parser.exit->delete(&parser.exit);
}

void initialize_internals(Maze* maze)
{
    maze->_internals = malloc(sizeof (struct maze_internals));
    maze->_internals->char_map = parser.char_map;
    maze->_internals->matrix = parser.matrix;
}
