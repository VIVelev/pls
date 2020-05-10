#ifndef UTILS_H
#define UTILS_H

#include <string.h>

#ifdef __APPLE__
#include <sys/syslimits.h>
#elif __linux__
#include <linux/limits.h>
#else
#error "Unsupported platform"
#endif

#define IS_CURR_DIR(name) (strcmp(name, ".") == 0)
#define IS_PREV_DIR(name) (strcmp(name, "..") == 0)
#define IS_CURR_PREV_DIR(name) (IS_CURR_DIR(name) || IS_PREV_DIR(name))
#define IS_HIDDEN(name) (name[0] == '.')

typedef char path_t[PATH_MAX];

int path_comparator(const void *a, const void *b);
void join_paths(const path_t path1, const path_t path2, path_t dest);

int push_back_dirs(const path_t parentdir, path_t *paths, int n);

#endif /* UTILS_H */
