#ifndef CHAR_MAP_H
#define CHAR_MAP_H

#include <stdbool.h>

typedef struct char_map CharMap;
struct char_map {
    char wall;
    char corridor;
    char path;
    char entrance;
    char exit;

    bool (*hasDistinctElements)(CharMap* self);
    void (*print)(CharMap* self);
    void (*delete)(CharMap** self);
};

extern const struct char_map_class {
    CharMap* (*new)();
} CharMapClass;

#endif
