#ifndef LS_H
#define LS_H

#include <stdbool.h>

#include "utils.h"

void ls(path_t *paths, unsigned int n,
        bool display_hidden, bool long_output, bool recursive);

#endif /* LS_H */
