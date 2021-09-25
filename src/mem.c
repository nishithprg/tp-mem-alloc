
#include "mem.h"
#include "common.h"
#include "mem_os.h"
#include <stdio.h>


static fb *fb_head;
static mem_fit_function_t *curr_fit;
// static al al_head;

void * init_fb_header(void * addr, size_t size, fb * next_free_block){
    fb * new_free_bloc_header = addr;
    new_free_bloc_header->taille_bloc = size;
    new_free_bloc_header->next = next_free_block;
    return (void *)new_free_bloc_header;
}

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
    mem_fit_function_t * arg = &mem_first_fit;
    mem_fit(arg);
    void * memory = get_memory_adr();
    fb_head = (fb *)memory;
    fb_head->taille_bloc = get_memory_size();
    fb_head->next = NULL;
    return;
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
// All allocations must produce aligned blocks
void *mem_alloc(size_t size) {
    fb * free_bloc_to_alloc = curr_fit(fb_head, size);
    size_t bloc_size = free_bloc_to_alloc->taille_bloc;

    // Insufficient block size
    if(free_bloc_to_alloc == NULL) return NULL; 

    fb * tmp = fb_head;
    fb * prec = NULL;
    while(tmp != NULL && tmp != free_bloc_to_alloc){
        prec = tmp;
        tmp = tmp->next;
    }

    if(prec == NULL)
        prec = tmp;
    size_t padding_size = 0;
    // Add padding
    // INCLUDE IN REPORT : we tried we modulo 4, line 72 writes 8 bytes to 4 alloc req and  hence overiting taille_bloc of fb_head hehe
    if(size % 8 != 0)
        padding_size = 8 - (size % 8); 
    // Check for enough place to add a Freeblock header and atleast one byte for allocation
    if((bloc_size - (size+padding_size)) < (sizeof(fb) + 1)){
        padding_size = bloc_size - size;
    }
    size_t offset = size+padding_size;
    // size_t offset = size;
    
    // Change fb_head if first bloc is allocated
    if(free_bloc_to_alloc == fb_head)
        fb_head = init_fb_header((void *)free_bloc_to_alloc+offset, bloc_size - offset, free_bloc_to_alloc->next);
    else
        // Add new updated free block to memory and return its address 
        prec->next = init_fb_header((void *)free_bloc_to_alloc+offset, bloc_size - offset, free_bloc_to_alloc->next);
    
    // Add extra bytes to align or extra bytes if not enough bytes for new free allocation block
    free_bloc_to_alloc->taille_bloc = offset;

    return (void *)free_bloc_to_alloc;
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
void mem_free(void *zone) {
    //Address of the zone that needs to be freed
    fb * tmp = fb_head;
    fb * prec = NULL;
    // We search where the zone to free is
    while(tmp != NULL && ((void *) tmp+tmp->taille_bloc < zone)){
        
        prec = tmp;
        tmp = tmp->next;
    }

    if(tmp == NULL)
        tmp = prec;


    int flg_contigue = 0;
    // Left fusion
    if((void *)tmp + tmp->taille_bloc == zone){
        flg_contigue = 1;
        init_fb_header(tmp, tmp->taille_bloc + ((al *) zone)->taille_bloc, tmp->next );

    }
    // Right fusion
    if(tmp->next != NULL){
        if(tmp->next == zone + ((al *) zone)->taille_bloc){
            if(flg_contigue){
                init_fb_header(tmp, tmp->taille_bloc + tmp->next->taille_bloc, tmp->next->next);
            }else{
                tmp->next = init_fb_header(zone, ((al *)zone)->taille_bloc + tmp->next->taille_bloc, tmp->next->next);
            }
            flg_contigue++;
        }
    }
    
    // No fusion
    if(!flg_contigue){
        tmp->next = init_fb_header(zone, ((al *) zone)->taille_bloc, tmp->next );
    }

        
    return;
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
// mem_show
//-------------------------------------------------------------
void mem_show(void (*print)(void *, size_t, int free)) {
    void * curr_byte_addr = get_memory_adr();
    fb * free_bloc_tmp = fb_head; 
    while(free_bloc_tmp != NULL || (curr_byte_addr < (get_memory_adr() + get_memory_size()))){
        if(curr_byte_addr == (void *)free_bloc_tmp){
            print((void *)free_bloc_tmp, free_bloc_tmp->taille_bloc, 1);
            curr_byte_addr += free_bloc_tmp->taille_bloc;
            free_bloc_tmp = free_bloc_tmp->next;
        } else {
            al * alloc_bloc_tmp = curr_byte_addr;
            print((void *)curr_byte_addr, alloc_bloc_tmp->taille_bloc, 0);
            curr_byte_addr += alloc_bloc_tmp->taille_bloc; 
        }
    } 
    return;
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t *mff) {
    curr_fit = mff;
    return;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
struct fb *mem_first_fit(struct fb *head, size_t size) {
    fb * tmp = head;
    while(tmp != NULL && tmp->taille_bloc < size){ // We want tmp->taille_bloc >= size to be true
        tmp = tmp->next;
    }
    if(tmp != NULL) return tmp;
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
