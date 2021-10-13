#include "common.h"
#include "mem.h"
#include "mem_os.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NB_TESTS 5
#define NB_MAX_STORES 100

void afficher_zone(void *adresse, size_t taille, int free) {
    printf("Zone %s, Adresse : %lu, Taille : %lu\n", free ? "libre" : "occupee",
           (unsigned long)adresse, (unsigned long)taille);
}

void my_free(void **mem) {
    if (*mem != NULL) {
        mem_free(*mem);
        // debug("Freed %lu\n", (unsigned long)*mem);
        *mem = NULL;
    }
}

static void *checked_alloc(size_t s) {
    void *result;

    assert((result = mem_alloc(s)) != NULL);
    // debug("Alloced %zu bytes at %p\n", s, result);
    return result;
}

int first = 1;
int nb_allocs = 0;
void *allocs[NB_MAX_STORES];

void reset() {
    first = 0;
    nb_allocs = 0;
}

void store_or_check(void *adr) {
    if (nb_allocs < NB_MAX_STORES) {
        if (first)
            allocs[nb_allocs++] = adr;
        else{
            assert(allocs[nb_allocs++] == adr);
        }
    }
}

void alloc_fun(int n) {
    void *a, *b, *c;
    if (n < 0)
        return;
    a = checked_alloc(5);
    store_or_check(a);
    b = checked_alloc(10);
    store_or_check(b);
    alloc_fun(n - 1);
    my_free(&a);
    c = checked_alloc(5);
    store_or_check(c);
    alloc_fun(n - 2);
    my_free(&c);
    my_free(&b);
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
    fprintf(stderr, "Test réalisant récursivement une allocation en gruyère "
                    "selon le modèle d'appel de fibonacci.\n"
                    "Définir DEBUG à la compilation pour avoir une sortie un "
                    "peu plus verbeuse."
                    "\n");
    for (int i = 0; i < NB_TESTS; i++) {
        debug("Issuing test number %d\n", i);
        alloc_fun(6);
        mem_show(afficher_zone);
        reset();
    }

    // TEST OK
    return 0;
}
