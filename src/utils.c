#define _BSD_SOURCE

#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/**
 * Compares two paths.
 * 
 * @param a, void pointer to the first path
 * @param b, void pointer to the second path
 * 
 * @return integer
 */
int path_comparator(const void *a, const void *b) {
    const path_t *path_a = (const path_t *)a;
    const path_t *path_b = (const path_t *)b;

    return strcmp(*path_a, *path_b);
}

/**
 * Joins two paths
 * 
 * @param path1
 * @param path2
 * @param dest, the resulting path will be placed here
 */
void join_paths(const path_t path1, const path_t path2, path_t dest) {
    if (strlen(path1) == 0) {
        strcpy(dest, path2);
        return;
    }

    if (strlen(path2) == 0) {
        strcpy(dest, path1);
        return;
    }

    if (!IS_CURR_DIR(path1) && strcmp(path1, "") != 0) {
        strcpy(dest, path1);
        if (path1[strlen(path1) - 1] != '/') {
            strcat(dest, "/");
        }
    } else {
        strcpy(dest, "");
    }

    strcat(dest, path2);
}

/**
 * Move the directories to the end of `paths` list.
 * 
 * For example:
 * Consider paths = [f, l, d, d, f, d, l]
 * where `f` is file, `l` is symbolic link and `d` is directory.
 * 
 * After `push_back_dirs` is called, on `paths`
 * the resultant `paths` will be [f, l, f, l, d, d, d]
 * 
 * @param parentdir, path to parent directory
 * @param paths, the list of paths
 * @param n, the length of `paths`
 * 
 * @return integer, the index of the first directory
 * in the resultant `paths`
 */
unsigned int push_back_dirs(const path_t parentdir, path_t *paths, unsigned int n) {
    if (n == 0) {
        return 0;
    }

    path_t fullpath;
    unsigned int i = 0;

    while (42) {
        join_paths(parentdir, paths[i++], fullpath);
        struct stat st_i;
        lstat(fullpath, &st_i);

        if (S_ISDIR(st_i.st_mode)) {
            --i;
            break;
        }

        if (i == n) {
            return n;
        }
    }

    for (register unsigned int j = i + 1; j < n; ++j) {
        join_paths(parentdir, paths[j], fullpath);
        struct stat st_j;
        lstat(fullpath, &st_j);

        if (!S_ISDIR(st_j.st_mode)) {
            path_t tmp;
            strcpy(tmp, paths[i]);
            strcpy(paths[i], paths[j]);
            strcpy(paths[j], tmp);
            ++i;
        }
    }

    return i;
}
