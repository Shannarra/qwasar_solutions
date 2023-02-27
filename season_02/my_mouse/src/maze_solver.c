#include "maze_solver.h"
#include "path_search_algo.h"
#include <stdio.h>
#include <unistd.h>

static void solve(const char* path);
const struct maze_solver MazeSolver = {
    .solve = &solve
};

void solve(const char* path)
{
    Maze* maze = MazeClass.fromPath(path);
    PathSearchAlgo* algo = PathSearchAlgoClass.new(maze);
    if (maze->valid) {
        algo->run(algo);
        if (!algo->found) dprintf(STDERR_FILENO, "%s\n", "Maze has no solution");
    }
    maze->print(maze);
    maze->delete(&maze);
    if (algo->found) printf("%u STEPS\n", algo->num_steps -1);
    algo->delete(&algo);
    puts("");
}
