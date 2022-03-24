#include "universe.h"

#include <inttypes.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
    int opt;
    bool toroidal = false;
    bool display = true;
    uint32_t gens = 100;
    char *ptr;
    FILE *input = stdin;
    FILE *output = stdout;
    while ((opt = getopt(argc, argv, "tsn:i:o:")) != -1) {
        switch (opt) {
        case 't': toroidal = true; break;
        case 's': display = false; break;
        case 'n': gens = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'i': input = fopen(optarg, "r"); break;
        case 'o': output = fopen(optarg, "w"); break;
        default: break;
        }
    }

    uint32_t rows = 0;
    uint32_t cols = 0;
    int ins = fscanf(input, "%" PRIu32 " %" PRIu32 "\n", &rows, &cols);
    if (ins != 2) {
        printf("Malformed Inputs\n");
        return 0;
    }
    Universe *a = uv_create(rows, cols, toroidal);
    Universe *b = uv_create(rows, cols, toroidal);
    bool pos = uv_populate(a, input);
    if (!pos) {
        printf("Malformed Inputs\n");
        return 0;
    }
    for (uint32_t i = 0; i < gens; i++) {
        if (display) {
            initscr();
            curs_set(FALSE);
            clear();
            for (uint32_t ii = 0; ii < uv_rows(a); ii++) {
                for (uint32_t jj = 0; jj < uv_cols(a); jj++) {
                    if (uv_get_cell(a, ii, jj))
                        mvprintw(ii, jj, "o");
                    else
                        mvprintw(ii, jj, ".");
                }
            }
            refresh();
            usleep(50000);
        }
        for (uint32_t ii = 0; ii < uv_rows(a); ii++) {
            for (uint32_t jj = 0; jj < uv_cols(a); jj++) {
                if (uv_get_cell(a, ii, jj)
                    && (uv_census(a, ii, jj) == 2 || uv_census(a, ii, jj) == 3))
                    uv_live_cell(b, ii, jj);
                else if (!uv_get_cell(a, ii, jj) && uv_census(a, ii, jj) == 3)
                    uv_live_cell(b, ii, jj);
                else
                    uv_dead_cell(b, ii, jj);
            }
        }
        Universe *temp = a;
        a = b;
        b = temp;
    }
    if (display)
        endwin();
    uv_print(a, output);
    uv_delete(a);
    uv_delete(b);
    fclose(input);
    fclose(output);
    return 0;
}
