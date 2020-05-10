#include "ls.h"
#include "lsdir.h"
#include "lsfiles.h"

#include <stdio.h>
#include <stdlib.h>

void ls(path_t *paths, int n,
        bool display_hidden, bool long_output, bool recursive) {

    /* Sort files */
    qsort(paths, n, sizeof(path_t), path_comparator);

    /* Move dirs to the back of the list */
    int diridx = push_back_dirs(".", paths, n);

    /* List files */
    lsfiles("", (const path_t *)paths, diridx, long_output);

    bool print_header = n > 1;
    /* Traverse the directories */
    for (register int i = diridx; i < n; ++i) {
        if (print_header) {
            if (diridx != 0 || i > diridx) {
                printf("\n");
            }
        }

        lsdir("", paths[i], print_header,
              display_hidden, long_output, recursive);
    }
}
