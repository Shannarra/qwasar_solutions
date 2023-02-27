#include "char_map.h"
#include <stdlib.h>
#include <stdio.h>

static CharMap* new();
const struct char_map_class CharMapClass = {
    .new = &new
};

static bool has_distinct_elements(CharMap* self);
static void print(CharMap* self);
static void delete(CharMap** self);
CharMap* new()
{
    CharMap* self = malloc(sizeof (CharMap));

    self->hasDistinctElements = &has_distinct_elements;
    self->print = &print;
    self->delete = &delete;
    return self;
}

bool has_distinct_elements(CharMap* self)
{
    char members[] = {self->wall, self->corridor, self->path, self->entrance, self->exit};
    for (unsigned char i = 0; i < 4; i++) {
        for (unsigned char j = i + 1; j < 5; j++) {
            if (members[i] == members[j]) return false;
        }
    }
    return true;
}

void print(CharMap* self)
{
    printf("%c%c%c%c%c", self->wall, self->corridor, self->path, self->entrance, self->exit);
}

void delete(CharMap** self)
{
    free(*self); *self = NULL;
}
