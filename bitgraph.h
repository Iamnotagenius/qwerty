#ifndef BITGRAPH
#define BITGRAPH
#include <stdint.h>
#include <limits.h>

int bitgraph_init_with_loops(uint32_t *buffer, uint32_t length);

static inline int bitgraph_link_directed(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst);
static inline int bitgraph_unlink_directed(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst);
static inline int bitgraph_link_symmetric(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst);
static inline int bitgraph_unlink_symmetric(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst);
static inline int bitgraph_check_adjacency(uint32_t *buffer, uint32_t length, uint32_t src, uint32_t dst);

#endif
