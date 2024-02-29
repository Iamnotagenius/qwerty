#include <stdint.h>
#include <stdio.h>

#define COMPILE_TIME_MAP

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
    buffer[0] = letters_to_mask("AQZS");
}
#endif

int main(int argc, char *argv[]) {
#ifndef COMPILE_TIME_MAP
    uint32_t map[26];
    init_qwerty_map(map);
#endif
    uint32_t mask = 
#ifdef COMPILE_TIME_MAP
        QWERTY_MAP[0];
#else
    map[0];
#endif
    printf("%08X\n", mask);
}
