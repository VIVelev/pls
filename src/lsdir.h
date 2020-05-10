#ifndef LSDIR_H
#define LSDIR_H

#include "utils.h"
#include <stdbool.h>

void lsdir(const path_t parentdir, const path_t dirpath, bool print_header,
           bool display_hidden, bool long_output, bool recursive);

#endif /* LSDIR_H */
