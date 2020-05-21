#include "lsdir.h"
#include "lsfiles.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Lists all files in a directory and recursively
 * traverses subdirectories if `recursive` is true.
 * 
 * @param parentdir, path to parent directory
 * @param dirpath, path/name of current directory
 * @param print_header, whether to print header (path to directory)
 * @param display_hidden, wheather to display hidden files
 * @param long_output, wheather to output detailed information for the files
 * @param recursive, wheather to traverse directories recursively
 */
void lsdir(const path_t parentdir, const path_t dirpath, bool print_header,
           bool display_hidden, bool long_output, bool recursive) {

    /* Get the full path */
    path_t fullpath;
    join_paths(parentdir, dirpath, fullpath);

    /* Try to open the directory */
    DIR *dir_p = opendir(fullpath);
    if (dir_p == NULL) {
        char msg_header[] = "ls: cannot open directory %s";
        char msg[strlen(msg_header) + sizeof(path_t)];

        sprintf(msg, msg_header, fullpath);
        perror((const char *)msg);
        return;
    }

    /* Get all the names of the files in the directory */
    path_t *entries_names = NULL;
    unsigned int nentries = 0;

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
    if (closedir(dir_p) == -1) {
        perror("closedir");
        return;
    }

    /* Move dirs to the back of the list */
    unsigned int diridx = push_back_dirs(fullpath, entries_names, nentries);

    /* Sort files */
    qsort(entries_names, diridx, sizeof(path_t), path_comparator);

    /* Sort dirs */
    qsort(entries_names + diridx, nentries - diridx, sizeof(path_t), path_comparator);

    if (print_header) {
        if (!IS_CURR_DIR(fullpath)) {
            printf("./");
        }
        printf("%s:\n", fullpath);
    }

    /* List all entries */
    lsfiles(fullpath, (const path_t *)entries_names, nentries, long_output);

    if (recursive) {
        for (register unsigned int i = diridx; i < nentries; ++i) {
            printf("\n");
            lsdir(fullpath, entries_names[i], true,
                  display_hidden, long_output, recursive);
        }
    }

    free(entries_names);
}
