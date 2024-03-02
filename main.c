#include <ctype.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qwerty.h"
#ifndef COMPILE_TIME_MAP
#define IS_ADJACENT(char1, char2) is_adjacent(adjacency_map, char1, char2)
#endif

#define READ_BUFFER_SIZE 4096

static const char *USAGE_MSG = "This program counts english words that can be typed using only adjacent keys on a QWERTY keyboard.\n"
                               "Usage: %s [-v | -q] [-i] [-s SEP | -0] FILE\n"
                               "where FILE is path to a dictionary (or - for stdin) with each word in a separate line\n"
                               "Options:\n"
                               "  -q     -- suppress warnings about invalid characters\n"
                               "  -v     -- print check result for each word\n"
                               "  -i     -- ignore non-letters\n"
                               "  -s SEP -- expect SEP character to be as a separator\n"
                               "  -0     -- expect NUL character to be as a separator\n"
                               "  -e     -- count empty words (a word is considered empty when it does not have any english letter)\n"
#ifdef COMPILE_TIME_MAP
                               "This executable is built with COMPILE_TIME_MAP defined\n"
#endif
                               ;

typedef enum {
    QUIET, NORMAL, VERBOSE
} verbose_level_t;

static inline bool has_verbose_level(verbose_level_t level, verbose_level_t check) {
    return level >= check;
}

int main(int argc, char *argv[]) {
    int opt;
    verbose_level_t verbosity = NORMAL;
    bool ignore_invalid = false;
    bool count_empty = false;
    uint8_t separator = '\n';
    /* I could use getopt_long but decided to stay POSIX-compatible */
    while ((opt = getopt(argc, argv, "hqvs:0ie")) != -1) {
        switch (opt) {
        case 'q': verbosity = QUIET; break;
        case 'v': verbosity = VERBOSE; break;
        case 's': separator = optarg[0]; break;
        case '0': separator = '\0';
        case 'i': ignore_invalid = true; break;
        case 'e': count_empty = true; break;
        default:
            fprintf(stderr, USAGE_MSG, argv[0]);
            return EXIT_FAILURE;
        }
    }
    if (optind >= argc) {
        fprintf(stderr, "%s: expected filepath\n", argv[0]);
        fprintf(stderr, USAGE_MSG, argv[0]);
        return EXIT_FAILURE;
    }
    FILE *dictionary = stdin;
    if (strcmp("-", argv[optind]) != 0) {
        dictionary = fopen(argv[optind], "r");
        if (dictionary == NULL) {
            perror(argv[0]);
            return EXIT_FAILURE;
        }
    }
    char buffer[READ_BUFFER_SIZE];
    buffer[0] = '\0';
#ifndef COMPILE_TIME_MAP
    uint32_t adjacency_map[26];
    init_qwerty_map(adjacency_map);
#endif

    size_t read = 1, count = 0, column = 0, line = 1;
    int32_t prev = -1;
    bool should_count = true, is_empty = true;
    setlocale(LC_CTYPE, "C"); /* ensure isalpha returns true only for english letters */
    while (!feof(dictionary)) {
        read = fread(buffer, sizeof(char), sizeof(buffer), dictionary);
        for (size_t i = 0; i < read; i++) {
            column++;
            if (buffer[i] == separator) {
                count += should_count && (count_empty >= is_empty);
                if (has_verbose_level(verbosity, VERBOSE) && !is_empty) {
                    if (should_count) {
                        fprintf(stderr, "Word on line %lu can be typed\n", line);
                    }
                    else {
                        fprintf(stderr, "Word on line %lu can't be typed\n", line);
                    }
                }
                column = 0;
                line++;
                prev = -1;
                should_count = true;
                is_empty = true;
                continue;
            }
            if (!isalpha(buffer[i])) {
                if (!ignore_invalid) {
                    if (has_verbose_level(verbosity, NORMAL)) {
                        fprintf(stderr, "Invalid char '%c' on line %lu at column %ld\n", buffer[i], line, column);
                    }
                    should_count = false;
                }
                continue;
            }
            else {
                is_empty = false;
            }
            if (prev != -1 && !IS_ADJACENT(prev, buffer[i])) {
                if (has_verbose_level(verbosity, VERBOSE)) {
                    fprintf(stderr, "%c is not adjacent to %c\n", prev, buffer[i]);
                }
                should_count = false;
            }
            prev = buffer[i];
        }
    }
    if (buffer[read - 1] != separator) {
        fprintf(stderr, "buffer[read - 1] = '%c'\n", buffer[read - 1]);
        count += should_count && (count_empty >= is_empty);
        if (has_verbose_level(verbosity, VERBOSE) && !is_empty) {
            if (should_count) {
                fprintf(stderr, "Word on line %lu can be typed\n", line);
            }
            else {
                fprintf(stderr, "Word on line %lu can't be typed\n", line);
            }
        }

    }

    printf("%lu\n", count);
}
