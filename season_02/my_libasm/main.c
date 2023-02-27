#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "./assembly_stubs.h"
// #undef the next line if you want to overwrite the testing functions
#define USE_TESTING_FUNCTIONS
#include "./testing_functions.h"

int main(void)
{
    test_strlen();
    test_strchr();
    test_memset();
    test_memcpy();
    test_strcmp();
    test_memmove();
    test_strncmp();
    test_strcasecmp();
    test_index();
    test_read();
    test_write();
}
