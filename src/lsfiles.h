#ifndef LSFILES_H
#define LSFILES_H

#include "utils.h"
#include <stdbool.h>

#define SIX_MONTHS 15770000

#define RED "\033[31m"
#define PINK "\033[35m"
#define WHITE "\033[37m"

void lsfiles(const path_t dirpath, const path_t *filepaths,
             int n, bool long_output);

#endif /* LSFILES_H */
