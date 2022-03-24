#include "universe.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
struct Universe {
    uint32_t rows;
    uint32_t cols;
    bool **grid;
    bool toroidal;
};

Universe *uv_create(uint32_t rows, uint32_t cols, bool toroidal) {
    Universe *god = malloc(sizeof(Universe));
    god->rows = rows;
    god->cols = cols;
    god->toroidal = toroidal;
    god->grid = (bool **) calloc(rows, sizeof(bool *));
    for (uint32_t r = 0; r < rows; r += 1) {
        god->grid[r] = (bool *) calloc(cols, sizeof(bool));
    }
    return god;
}

void uv_delete(Universe *u) {
    for (uint32_t r = 0; r < uv_rows(u); r += 1) {
        free(u->grid[r]);
    }
    free(u->grid);
    free(u);
}

uint32_t uv_rows(Universe *u) {
    return u->rows;
}

uint32_t uv_cols(Universe *u) {
    return u->cols;
}

void uv_live_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= uv_rows(u))
        return;
    if (c >= uv_cols(u))
        return;
    u->grid[r][c] = true;
}

void uv_dead_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= uv_rows(u))
        return;
    if (c >= uv_cols(u))
        return;
    u->grid[r][c] = false;
}

bool uv_get_cell(Universe *u, uint32_t r, uint32_t c) {
    if (r >= uv_rows(u))
        return false;
    if (c >= uv_cols(u))
        return false;
    return u->grid[r][c];
}

bool uv_populate(Universe *u, FILE *infile) {
    uint32_t num1 = 0;
    uint32_t num2 = 0;
    while (true) {
        int ins = fscanf(infile, "%" PRIu32 " %" PRIu32 "\n", &num1, &num2);
        if (ins == EOF)
            break;
        if (ins != 2)
            return false;
        uv_live_cell(u, num1, num2);
        if (!uv_get_cell(u, num1, num2))
            return false;
    }
    return true;
}

uint32_t uv_census(Universe *u, uint32_t r, uint32_t c) {
    uint32_t cnt = 0;
    if (!(u->toroidal)) {
        if (r < uv_rows(u) - 1 && uv_get_cell(u, r + 1, c))
            cnt++;
        if (r > 0 && uv_get_cell(u, r - 1, c))
            cnt++;
        if (c < uv_cols(u) - 1 && uv_get_cell(u, r, c + 1))
            cnt++;
        if (c > 0 && uv_get_cell(u, r, c - 1))
            cnt++;
        if (r < uv_rows(u) - 1 && c < uv_cols(u) - 1 && uv_get_cell(u, r + 1, c + 1))
            cnt++;
        if (r > 0 && c > 0 && uv_get_cell(u, r - 1, c - 1))
            cnt++;
        if (r > 0 && c < uv_cols(u) - 1 && uv_get_cell(u, r - 1, c + 1))
            cnt++;
        if (r < uv_rows(u) - 1 && c > 0 && uv_get_cell(u, r + 1, c - 1))
            cnt++;
    } else {
        cnt += uv_get_cell(u, (r + 1) % uv_rows(u), c);
        cnt += uv_get_cell(u, r, (c + 1) % uv_cols(u));
        cnt += uv_get_cell(u, (r + 1) % uv_rows(u), (c + 1) % uv_cols(u));
        if (r == 0) {
            cnt += uv_get_cell(u, uv_rows(u) - 1, c);
            cnt += uv_get_cell(u, uv_rows(u) - 1, (c + 1) % uv_cols(u));
        } else if (r > 0) {
            cnt += uv_get_cell(u, r - 1, c);
            cnt += uv_get_cell(u, r - 1, (c + 1) % uv_cols(u));
        }
        if (c == 0) {
            cnt += uv_get_cell(u, r, uv_cols(u) - 1);
            cnt += uv_get_cell(u, (r + 1) % uv_rows(u), uv_cols(u) - 1);

        } else if (c > 0) {
            cnt += uv_get_cell(u, r, c - 1);
            cnt += uv_get_cell(u, (r + 1) % uv_rows(u), c - 1);
        }
        if (r == 0 && c == 0)
            cnt += uv_get_cell(u, uv_rows(u) - 1, uv_cols(u) - 1);
        else if (r == 0 && c > 0)
            cnt += uv_get_cell(u, uv_rows(u) - 1, c - 1);
        else if (r > 0 && c == 0)
            cnt += uv_get_cell(u, r - 1, uv_cols(u) - 1);
        else if (r > 0 && c > 0)
            cnt += uv_get_cell(u, r - 1, c - 1);
    }
    return cnt;
}

void uv_print(Universe *u, FILE *outfile) {
    for (uint32_t i = 0; i < uv_rows(u); i++) {
        for (uint32_t j = 0; j < uv_cols(u); j++) {
            if (uv_get_cell(u, i, j))
                fprintf(outfile, "o");
            else
                fprintf(outfile, ".");
        }
        fprintf(outfile, "\n");
    }
}
