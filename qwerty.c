#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>

#include "qwerty.h"

/* #define COMPILE_TIME_MAP */

/* Since english alphabet is only 26 characters long,
 * I can build an adjacency matrix using uint32_t as entire rows
 * and store the edges as single bits */

#ifndef COMPILE_TIME_MAP
static inline uint32_t letters_to_mask(const char *set) {
    uint32_t mask = 0;
    const char *cur = set;
    while (*cur != '\0') {
        mask |= 1 << (*cur - 'A');
        cur++;
    }
    return mask;
}
void init_qwerty_map(uint32_t buffer[26]) {
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

bool is_adjacent(const uint32_t *graph, uint8_t char1, uint8_t char2) {
    return graph[to_index(char1)] & (1 << to_index(char2));
}

