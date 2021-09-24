
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
    void * curr_byte_addr = get_memory_adr();
    fb * free_bloc_tmp = fb_head; 
    while(free_bloc_tmp != NULL && (curr_byte_addr != get_memory_adr() + get_memory_size())){
        if(curr_byte_addr == (void *)free_bloc_tmp){
            print((void *)free_bloc_tmp, free_bloc_tmp->taille_bloc, 1);
            curr_byte_addr = (void *)free_bloc_tmp->next + (free_bloc_tmp->taille_bloc - sizeof(size_t));
        } else {
            
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
