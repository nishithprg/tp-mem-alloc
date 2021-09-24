
#include "mem.h"
#include "common.h"
#include "mem_os.h"
#include <stdio.h>

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
static fb *fb_head;
// static al al_head;

void mem_init() {
    // mem_fit_function_t * arg = &mem_first_fit;
    // mem_fit(arg);
    void * memory = get_memory_adr();
    fb_head = (fb *)memory;
    fb_head->taille_bloc = get_memory_size();
    fb_head->next = NULL;
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
    void * tmp = get_memory_adr();
    while(tmp != NULL && tmp != get_memory_adr() + get_memory_size()){
        if(get_bit_bloc((size_t)tmp)){
            fb * fb_tmp = (fb *)tmp;
            print(tmp, fb_tmp->taille_bloc, 1);
            tmp = fb_tmp->next;
        } else {
            al * al_tmp = (al *) tmp;
            print(tmp, al_tmp->taille_bloc, 0);
            tmp = tmp + ((al *)tmp)->taille_bloc;
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
