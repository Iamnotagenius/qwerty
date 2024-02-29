#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* #define COMPILE_TIME_MAP */

#ifdef COMPILE_TIME_MAP
/* thanks, vim. (:set nrformats+=alpha) */
#define A (1 << 0)
#define B (1 << 1)
#define C (1 << 2)
#define D (1 << 3)
#define E (1 << 4)
#define F (1 << 5)
#define G (1 << 6)
#define H (1 << 7)
#define I (1 << 8)
#define J (1 << 9)
#define K (1 << 10)
#define L (1 << 11)
#define M (1 << 12)
#define N (1 << 13)
#define O (1 << 14)
#define P (1 << 15)
#define Q (1 << 16)
#define R (1 << 17)
#define S (1 << 18)
#define T (1 << 19)
#define U (1 << 20)
#define V (1 << 21)
#define W (1 << 22)
#define X (1 << 23)
#define Y (1 << 24)
#define Z (1 << 25)

static const uint32_t QWERTY_MAP[26] = {
    A | W | Q | Z | S,
    B | G | H | V | N,
    C | D | F | X | V,
    D | E | R | S | F | X | C,
    E | W | R | S | D,
    F | R | T | D | G | C | V,
    G | T | Y | F | H | V | B,
    H | Y | U | G | J | B | N,
    I | U | O | J | K,
    J | U | I | H | K | N | M,
    K | I | O | J | L | M,
    L | O | P | K,
    M | J | K | N,
    N | H | J | B | M,
    O | I | P | K | L,
    P | O | L,
    Q | W | A,
    R | E | T | D | F,
    S | W | E | A | D | Z | X,
    T | R | Y | F | G,
    U | Y | I | H | J,
    V | F | G | C | B,
    W | Q | E | A | S,
    X | S | D | Z | C,
    Y | T | U | G | H,
    Z | A | S | X,
};

#else /* Runtime version */
static inline uint32_t letters_to_mask(const char *set) {
    uint32_t mask = 0;
    const char *cur = set;
    while (*cur != '\0') {
        mask |= 1 << (*cur - 'A');
        cur++;
    }
    return mask;
}
static inline void init_qwerty_map(uint32_t buffer[26]) {
    buffer[0] = letters_to_mask("AWQZS");
    buffer[1] = letters_to_mask("BGHVN");
    buffer[2] = letters_to_mask("CDFXV");
    buffer[3] = letters_to_mask("DERSFXC");
    buffer[4] = letters_to_mask("EWRSD");
    buffer[5] = letters_to_mask("FRTDGCV");
    buffer[6] = letters_to_mask("GTYFHVB");
    buffer[7] = letters_to_mask("HYUGJBN");
    buffer[8] = letters_to_mask("IUOJK");
    buffer[9] = letters_to_mask("JUIHKNM");
    buffer[10] = letters_to_mask("KIOJLM");
    buffer[11] = letters_to_mask("LOPK");
    buffer[12] = letters_to_mask("MJKN");
    buffer[13] = letters_to_mask("NHJBM");
    buffer[14] = letters_to_mask("OIPKL");
    buffer[15] = letters_to_mask("POL");
    buffer[16] = letters_to_mask("QWA");
    buffer[17] = letters_to_mask("RETDF");
    buffer[18] = letters_to_mask("SWEADZX");
    buffer[19] = letters_to_mask("TRYFG");
    buffer[20] = letters_to_mask("UYIHJ");
    buffer[21] = letters_to_mask("VFGCB");
    buffer[22] = letters_to_mask("WQEAS");
    buffer[23] = letters_to_mask("XSDZC");
    buffer[24] = letters_to_mask("YTUGH");
    buffer[25] = letters_to_mask("ZASX");

}
#endif

static inline uint32_t to_index(uint8_t c) {
    return toupper(c) - 'A';
}

bool is_adjacent(uint32_t *graph, uint8_t char1, uint8_t char2) {
    return graph[to_index(char1)] & (1 << to_index(char2));
}

static const char *USAGE_MSG = "This program counts english words that can be typed using only adjacent keys.\n"
                               "Usage: %s FILE\n"
                               "where FILE is path to a dictionary with each word in a separate line\n"
                               "- stands for stdin\n";

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, USAGE_MSG, argv[0]);
        return 1;
    }
    if (strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0 || strcmp("-?", argv[1]) == 0) {
        fprintf(stderr, USAGE_MSG, argv[0]);
        return 0;
    }
    FILE *dictionary = stdin;
    if (strcmp("-", argv[1]) != 0) {
        dictionary = fopen(argv[1], "r");
    } 
    size_t size = 3;
    char buffer[size];
#ifdef COMPILE_TIME_MAP
    const uint32_t *adjacency_map = QWERTY_MAP;
#else
    uint32_t adjacency_map[26];
    init_qwerty_map(adjacency_map);
#endif

    size_t read, count = 0, column = 0, line = 1;
    int32_t prev = -1;
    bool should_count = true;
    while (!feof(dictionary)) {
        read = fread(buffer, sizeof(char), sizeof(buffer) - 1, dictionary);
        buffer[read] = '\0';
        /* printf("'%s' read (%ld bytes)\n", buffer, read); */
        for (size_t i = 0; i < read; i++) {
            column++;
            if (buffer[i] == '\n') {
                count += should_count;
                column = 0;
                line++;
                prev = -1;
                should_count = true;
                continue;
            }
            if (!isalpha(buffer[i])) {
                fprintf(stderr, "Invalid char '%c' on line %lu at pos %ld\n", buffer[i], line, column);
                should_count = false;
                continue;
            }
            if (prev == -1) {
                prev = buffer[i];
                continue;
            }
        }
    }
}
