#include <stdio.h>
#include <stdlib.h>

#include "my_curl.h"

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        fprintf(stderr, "%s\n", "my_curl takes exactly one argument");
        return EXIT_FAILURE;
    }
    return my_curl(argv[1]);
}
