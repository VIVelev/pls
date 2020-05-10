#include <stdio.h>
#include <unistd.h>

#include "ls.h"

int main(int argc, char **argv) {
    bool display_hidden = false,
         long_output = false,
         recursive = false;

    char ch;
    while ((ch = getopt(argc, argv, "AlR")) != -1) {
        switch (ch) {
        case 'A':
            display_hidden = true;
            break;
        case 'l':
            long_output = true;
            break;
        case 'R':
            recursive = true;
            break;
        default:
            printf("Usgae: ...\n");
            break;
        }
    }

    argc -= optind;
    argv += optind;

    if (argc) {
        path_t paths[argc];
        for (register int i = 0; i < argc; ++i) {
            strcpy(paths[i], argv[i]);
        }

        ls(paths, argc, display_hidden, long_output, recursive);

    } else {
        path_t paths[1];
        strcpy(paths[0], ".");

        ls(paths, 1, display_hidden, long_output, recursive);
    }

    return 0;
}
