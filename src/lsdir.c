#include "lsdir.h"
#include "lsfiles.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

void lsdir(const path_t parentdir, const path_t dirpath, bool print_header,
           bool display_hidden, bool long_output, bool recursive) {

    /* Get the full path */
    path_t fullpath;
    join_paths(parentdir, dirpath, fullpath);

    /* Try to open the directory */
    errno = 0;
    DIR *dir_p = opendir(fullpath);
    if (errno != 0) {
        char msg[sizeof(path_t) + 26];
        sprintf(msg, "ls: cannot open directory %s", fullpath);
        perror((const char *)msg);
        return;
    }

    /* Get all the names of the files in the directory */
    path_t *entries_names = NULL;
    int nentries = 0;

    struct dirent *entry;
    while ((entry = readdir(dir_p)) != NULL) {
        if (!IS_CURR_PREV_DIR(entry->d_name)) {
            if (IS_HIDDEN(entry->d_name) && !display_hidden) {
                continue;
            }

            entries_names = (path_t *)realloc(entries_names, sizeof(path_t) * ++nentries);
            strcpy(entries_names[nentries - 1], entry->d_name);
        }
    }
    closedir(dir_p);

    /* Sort files */
    qsort(entries_names, nentries, sizeof(path_t), path_comparator);

    /* Split files and directories */
    int diridx = push_back_dirs(fullpath, entries_names, nentries);

    if (print_header || recursive) {
        printf("%s:\n", fullpath);
    }

    /* List all entries */
    lsfiles(fullpath, (const path_t *)entries_names, nentries, long_output);

    if (recursive) {
        for (register int i = diridx; i < nentries; ++i) {
            printf("\n");
            lsdir(fullpath, entries_names[i], print_header,
                  display_hidden, long_output, recursive);
        }
    }

    free(entries_names);
}
