
#include "mem.h"
#include "common.h"
#include "mem_os.h"
#include <stdio.h>

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
static fb fb_head;
static al al_head;

static int set_bit_bloc_alloc(size_t taille_bloc){
    return taille_bloc & 0xFFFF;
}

static int set_bit_bloc_free(size_t taille_bloc){
    return taille_bloc & (0xFFFF >> 1);
}

static int get_bit_bloc(size_t taille_bloc){
    return taille_bloc >> sizeof(size_t) - 1;
}

void mem_init() {
    mem_fit(mem_fit_first);
    fb_head.taille_bloc_libre = set_bit_bloc_free(get_memory_size());
    fb_head.next = (fb *)get_memory_adr() + 1;
    return;
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void *mem_alloc(size_t size) {
    
    return NULL;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void *zone) {
    /* A COMPLETER */
    return;
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
    void * tmp = fb_head.next;
    while(tmp != NULL || tmp == get_memory_adr()){
        if(get_bit_bloc(*(int *)tmp){
            fb * fb_tmp = (fb *)tmp;
            printf(tmp, fb_tmp->taille_bloc, 1);
            tmp = tmp + sizeof(size_t);
        } else {
            al * al_tmp = (al *) tmp;
            printf(tmp, al_tmp->taille_bloc, 0);
            tmp = tmp + (size_t) tmp->taille_bloc;
        }
    } 
    return;
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t *mff) {
    /* A COMPLETER */
    return;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
struct fb *mem_first_fit(struct fb *head, size_t size) {
    /* A COMPLETER */
    return NULL;
}
//-------------------------------------------------------------
struct fb *mem_best_fit(struct fb *head, size_t size) {
    /* A COMPLETER */
    return NULL;
}
//-------------------------------------------------------------
struct fb *mem_worst_fit(struct fb *head, size_t size) {
    /* A COMPLETER */
    return NULL;
}
