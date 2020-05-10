#include "ls.h"
#include "lsdir.h"
#include "lsfiles.h"

#include <stdio.h>
#include <stdlib.h>

/**
 * Lists command line arguments.
 * 
 * @param paths, passed arguments
 * @param n, the number of arguments
 * @param display_hidden, wheather to display hidden files
 * @param long_output, wheather to output detailed information for the files
 * @param recursive, wheather to traverse directories recursively
 */
void ls(path_t *paths, unsigned int n,
        bool display_hidden, bool long_output, bool recursive) {

    /* Move dirs to the back of the list */
    unsigned int diridx = push_back_dirs("", paths, n);

    /* Sort files */
    qsort(paths, diridx, sizeof(path_t), path_comparator);

    /* Sort dirs */
    qsort(paths + diridx, n - diridx, sizeof(path_t), path_comparator);

    /* List files */
    lsfiles("", (const path_t *)paths, diridx, long_output);

    bool print_header = n > 1;
    /* Traverse the directories */
    for (register unsigned int i = diridx; i < n; ++i) {
        if (print_header) {
            if (diridx != 0 || i > diridx) {
                printf("\n");
            }
        }

        lsdir("", paths[i], print_header,
              display_hidden, long_output, recursive);
    }
}
