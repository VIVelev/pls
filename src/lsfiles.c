#define _BSD_SOURCE

#include "lsfiles.h"
#include "utils.h"

#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

/****************************************************************
 * Private lsunary functions - prototypes                       *
 ****************************************************************/

static char _get_filetype(unsigned int m);

static const char *_months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                                "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

/****************************************************************
 * Public lsunary functions                                     *
 ****************************************************************/

/**
 * Lists files (display output to the stdout)
 * 
 * @param dirpath, the path to the directory containing the files
 * @param filepaths, names/paths of the files
 * @param n, number of files to list
 * @param long_output, wheather to output detailed information for the files
 */
void lsfiles(const path_t dirpath, const path_t *filepaths,
             unsigned int n, bool long_output) {

    if (n == 0) {
        return;
    }

    /* If long output and in directory print total */
    if (long_output && strlen(dirpath)) {
        /* Total number of 1024 blocks allocated for the files */
        unsigned long total_blocks = 0;

        for (register unsigned int i = 0; i < n; ++i) {
            path_t fullpath;
            join_paths(dirpath, filepaths[i], fullpath);

            struct stat st;
            if (lstat(fullpath, &st) != 0)
                continue;

            total_blocks += st.st_blocks;
        }

        printf("total %lu\n", total_blocks >> 1);
    }

    for (register unsigned int i = 0; i < n; ++i) {
        path_t fullpath;
        join_paths(dirpath, filepaths[i], fullpath);

        struct stat st;
        if (lstat(fullpath, &st) != 0) {
            char msg[sizeof(path_t) + 18];
            sprintf(msg, "ls: cannot access %s", filepaths[i]);
            perror(msg);
            continue;
        }

        char type = _get_filetype(st.st_mode);
        printf("%c", type);

        if (long_output) {
            /* Printf user/group rights */
            printf("%c", (S_IRUSR & st.st_mode) ? 'r' : '-');
            printf("%c", (S_IWUSR & st.st_mode) ? 'w' : '-');
            printf("%c", (S_IXUSR & st.st_mode) ? 'x' : '-');
            printf("%c", (S_IRGRP & st.st_mode) ? 'r' : '-');
            printf("%c", (S_IWGRP & st.st_mode) ? 'w' : '-');
            printf("%c", (S_IXGRP & st.st_mode) ? 'x' : '-');
            printf("%c", (S_IROTH & st.st_mode) ? 'r' : '-');
            printf("%c", (S_IWOTH & st.st_mode) ? 'w' : '-');
            printf("%c ", (S_IXOTH & st.st_mode) ? 'x' : '-');

            /* Print number of hard links */
            printf("%2lu ", (unsigned long)st.st_nlink);

            /* Print user name and group name */
            printf("%s ", getpwuid(st.st_uid)->pw_name);
            printf(" ");
            printf("%s ", getgrgid(st.st_gid)->gr_name);

            /* Print file size n bytes */
            printf("%5llu ", (unsigned long long)st.st_size);

            /* Print file time */
            struct tm *file_tm = localtime(&st.st_ctime);

            printf("%s ", _months[file_tm->tm_mon]);
            printf("%2d ", file_tm->tm_mday);

            if (difftime(time(NULL), mktime(file_tm)) < SIX_MONTHS) {
                printf("%02d:%02d ", file_tm->tm_hour, file_tm->tm_min);
            } else {
                printf(" %4d ", 1900 + file_tm->tm_year);
            }

        } else {
            printf(" ");
        }

        /* Print file name */
        if (type == 'd') {
            printf("%s/\n", filepaths[i]);

        } else if (type == 'l') {
            printf(PINK "%s" WHITE "@", filepaths[i]);

            if (long_output) {
                char original_path[NAME_MAX];
                int chars_read = readlink(filepaths[i], original_path, NAME_MAX - 1);

                original_path[chars_read] = '\0';
                printf(" -> %s\n", original_path);
            }

        } else if (((S_IXUSR & st.st_mode) || (S_IXGRP & st.st_mode) || (S_IXOTH & st.st_mode))) {
            printf(RED "%s" WHITE "*\n", filepaths[i]);

        } else {
            printf("%s\n", filepaths[i]);
        }
    }
}

/****************************************************************
 * Private lsunary functions                                    *
 ****************************************************************/

/**
 * Returns the type of the file represented as a char.
 * (according to the POSIX standard)
 * 
 * @param m, the stat mode of the file (st.st_mode)
 * @return char, the type
 */
static char _get_filetype(unsigned int m) {
    if (S_ISREG(m))
        return '-';
    if (S_ISDIR(m))
        return 'd';
    if (S_ISCHR(m))
        return 'c';
    if (S_ISBLK(m))
        return 'b';
    if (S_ISFIFO(m))
        return 'p';
    if (S_ISLNK(m))
        return 'l';
    if (S_ISSOCK(m))
        return 's';

    return 'e';
}
