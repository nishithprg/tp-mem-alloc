#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ALLOC (1 << 10)
#define NB_TESTS 10

static void *allocs[MAX_ALLOC];

static int make_test() {
    int nb_alloc = 0;
    int i = 1;
    // On remplit la mémoire de blocs de taille croissante
    debug("Issuing a sequence of size increasing mallocs, starting from 0\n");
    while ((i < MAX_ALLOC) && ((allocs[i] = mem_alloc(i)) != NULL)) {
        i++;
    }
    i--;
    debug("Alloced up to %d bytes at %p\n", i, allocs[i]);
    nb_alloc = i;
    // On vide
    debug("Freeing all allocated memory\n");
    while (i >= 1) {
        mem_free(allocs[i]);
        // debug("Freed %p\n", allocs[i]);
        i--;
    }
    return nb_alloc;
}

int main(int argc, char *argv[]) {
    mem_init();
    if (argc == 2) {
        if (strcmp(argv[1], "first") == 0) {
            mem_fit(mem_first_fit);
            printf("Stratégie first fit\n");
        } else if (strcmp(argv[1], "best") == 0) {
            mem_fit(mem_best_fit);
            printf("Stratégie best fit\n");
        } else if (strcmp(argv[1], "worst") == 0) {
            mem_fit(mem_worst_fit);
            printf("Stratégie worst fit\n");
        }
    }
    
    int nb_alloc;
    nb_alloc = 0;
    fprintf(stderr, "Test réalisant des series d'allocations / désallocations "
                    "en ordre LIFO\n"
                    "Définir DEBUG à la compilation pour avoir une sortie un "
                    "peu plus verbeuse."
                    "\n");
    nb_alloc = make_test();
    for (int i = 0; i < NB_TESTS; i++) {
        // Teste si non idempotent !
        assert(make_test() == nb_alloc);
    }

    // TEST OK
    return 0;
}
