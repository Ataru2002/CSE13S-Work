#include "batcher.h"

void comparator(Stats *stats, uint32_t *A, uint32_t x, uint32_t y) {
    if (cmp(stats, A[x], A[y]) == 1) {
        swap(stats, &A[x], &A[y]);
    }
}

uint32_t count_bits(uint32_t in) {
    uint32_t cnt = 0;
    while (in > 0) {
        cnt++;
        in /= 2;
    }
    return cnt;
}

void batcher_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    if (n == 0)
        return;
    uint32_t t = count_bits(n);
    uint32_t p = 1 << (t - 1);

    while (p > 0) {
        uint32_t q = 1 << (t - 1);
        uint32_t r = 0;
        uint32_t d = p;
        while (d > 0) {
            for (uint32_t i = 0; i < n - d; i++) {
                if ((i & p) == r)
                    comparator(stats, A, i, i + d);
            }
            d = q - p;
            q >>= 1;
            r = p;
        }
        p >>= 1;
    }
}
