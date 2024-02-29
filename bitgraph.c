#include <cstdint>
#include <stdint.h>
#include <limits.h>

/* 
 * I use adjacency matrix for storing graph.
 * While the graph is sparse: the number of vertices is 26
 * and each vertex has at most 5 edges.
 * Storing each relation as one bit is the most efficient way,
 * because it uses bit operators to query if two vertices are adjacent with only one memory lookup.
 * Although, this approach would work up only up to 64 vertices,
 * then it would make sense to store a graph using adjacency list
 */

#define ERR_OUT_OF_BOUNDS 1

int bitgraph_init_with_loops(uint32_t *buffer, uint32_t length) {
    if (length > sizeof(uint32_t)) {
        return ERR_OUT_OF_BOUNDS;
    }
    uint32_t term = length == sizeof(uint32_t) * CHAR_BIT ? 0 : 1 << length;
    for (uint32_t mask = 1, i = 0; i < length; mask <<= 1, i++) {
        buffer[i] = mask;
    }
    return 0;
}

static inline int bitgraph_link_directed(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst) {
    if (length > sizeof(uint32_t) || src >= length || dst >= length) {
        return ERR_OUT_OF_BOUNDS;
    }
    buffer[src] |= 1 << dst;
    return 0;
}

static inline int bitgraph_unlink_directed(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst) {
    if (length > sizeof(uint32_t) || src >= length || dst >= length) {
        return ERR_OUT_OF_BOUNDS;
    }
    buffer[src] &= ~(1 << dst);
    return 0;
}

static inline int bitgraph_link_symmetric(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst) {
    if (length > sizeof(uint32_t) || src >= length || dst >= length) {
        return ERR_OUT_OF_BOUNDS;
    }
    buffer[src] |= 1 << dst;
    buffer[dst] |= 1 << src;
    return 0;
}

static inline int bitgraph_unlink_symmetric(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst) {
    if (length > sizeof(uint32_t) || src >= length || dst >= length) {
        return ERR_OUT_OF_BOUNDS;
    }
    buffer[src] &= ~(1 << dst);
    buffer[dst] &= ~(1 << src);
    return 0;
}

static inline int bitgraph_check_adjacency(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst) {
    if (length > sizeof(uint32_t) || src >= length || dst >= length) {
        return -ERR_OUT_OF_BOUNDS;
    }
    return buffer[src] & (1 << dst);
}
