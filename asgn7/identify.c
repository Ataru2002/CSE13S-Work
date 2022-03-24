#include "node.h"
#include "pq.h"
#include "ht.h"
#include "speck.h"
#include "bv.h"
#include "bf.h"
#include "text.h"
#include "parser.h"
#include "metric.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define min(a, b) (a < b) ? a : b

void usage() {
    fprintf(stderr,
        "Synopsis\n"
        "   Performs an author authentication to find which author is the closest to the subject "
        "text\n"
        "Usage\n"
        "   ./identify [Options]\n"
        "Options\n"
        "   -d database     Specifies the database file to compare the text (Default: lib.db)\n"
        "   -n noisefile    Specifies the noise file for filtering words (Default: noise.txt)\n"
        "   -k matches      Specifies the number of matches (Default: 5)\n"
        "   -l limit        Specifies the noise limit (Default: 100)\n"
        "   -e              Specifies the metric to Euclidean distance, which is also the default "
        "metric\n"
        "   -m              Specifies the metric to Manhattan distance\n"
        "   -c              Specifies the metric to Cosine distance\n"
        "   -h              Display program's synopsis\n");
}

int main(int argc, char **argv) {
    int opt;
    char *dataf = "lib.db";
    char *noisef = "noise.txt";
    uint32_t matches = 5;
    uint32_t filter = 100;
    Metric metric = EUCLIDEAN;
    char *ptr;
    while ((opt = getopt(argc, argv, "d:n:k:l:emch")) != -1) { //get the command
        switch (opt) {
        case 'd': dataf = optarg; break;
        case 'n': noisef = optarg; break;
        case 'k': matches = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'l': filter = (uint32_t) strtoul(optarg, &ptr, 10); break;
        case 'e': metric = EUCLIDEAN; break;
        case 'm': metric = MANHATTAN; break;
        case 'c': metric = COSINE; break;
        case 'h':
            usage();
            return 0;
            break;
        default: return EXIT_FAILURE; break;
        }
    }
    FILE *data = fopen(dataf, "r"); //data file
    FILE *noise = fopen(noisef, "r"); //noise file
    if (data == NULL) { //check if file is open properly
        printf("Can't open the database\n");
        return EXIT_FAILURE;
    }
    if (noise == NULL) {
        printf("Can't open the noise\n");
        return EXIT_FAILURE;
    }
    FILE *limitnoise = tmpfile(); //limit noise file
    char *inserts = malloc(1000 * sizeof(char));
    for (uint32_t i = 0; i < filter;
         i++) { //loop the noise file and reads the words base on the limit noise
        fgets(inserts, 1000, noise);
        fprintf(limitnoise, "%s\n", inserts); //insert the noise into the limit noise file
    }
    rewind(limitnoise);
    Text *ban = text_create(limitnoise, NULL); //this will be the noise
    Text *subject = text_create(stdin, ban); //this is the subject text filtered by the noise
    uint32_t cases;
    fscanf(data, "%u", &cases);
    PriorityQueue *pq = pq_create(cases);
    char *author = malloc(1000 * sizeof(char));
    char *path = malloc(1000 * sizeof(char));
    fgets(path, 1000, data);
    uint32_t cnt = 0;
    for (uint32_t i = 0; i < cases * 2; i++) { //read through the database
        if (i % 2) {
            fgets(path, 1000, data);
            path[strlen(path) - 1] = '\0';
            FILE *authors = fopen(path, "r");
            if (authors == NULL)
                continue;
            Text *source
                = text_create(authors, ban); //this is the author's text filtered by the noise
            enqueue(pq, author,
                text_dist(source, subject, metric)); //enqueue the distance to the priority queue
            cnt++;
            fclose(authors);
            text_delete(&source);
        } else {
            fgets(author, 1000, data);
            author[strlen(author) - 1] = '\0';
        }
    }
    uint32_t loops = min(matches, cnt);
    printf("Top %u, metric: %s, noise limit: %u\n", matches, metric_names[metric], filter);
    for (uint32_t i = 1; i <= loops; i++) { //print the authors
        char *outputa;
        double dist;
        dequeue(pq, &outputa, &dist);
        printf("%u) %s [%0.15f]\n", i, outputa, dist);
    }
    //clean up
    pq_delete(&pq);
    text_delete(&ban);
    text_delete(&subject);

    free(author);
    free(path);
    free(inserts);
    fclose(data);
    fclose(noise);
    fclose(limitnoise);
    return 0;
}
