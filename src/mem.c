
#include "mem.h"
#include "common.h"
#include "mem_os.h"
#include <stdio.h>

// Change PROC_ARC to 4 only if executing on 32 bits systems.
#define PROC_ARC 8
// 1 if adress are ascending, 0 otherwise; important for adress comparision
int adress_order;

// Analyse adress order on the executing machine]
void set_adress_order(){
    void * memory = get_memory_adr();
    if(((memory+1) - memory) > 0)
        adress_order = 1;
    else 
        adress_order = 0;
}

// Return 1 if adr1 comes before adr2, 0 otherwise
int compare_adresses(const void * adr1, const void * adr2){
    if(adress_order)
        return adr1 < adr2;
    else
        return adr2 < adr1;
}

// Return address offset bytes before adr1
void * address_substraction(void * adr1, size_t offset){
    if(adress_order)
        return adr1 - offset;
    else
        return adr1 + offset;
}

// Function creates a free block header
void * init_fb_header(void * addr, size_t size, fb * next_free_block){
    if(size == 0) return next_free_block;
    fb * new_free_bloc_header = addr;
    new_free_bloc_header->taille_bloc = size;
    new_free_bloc_header->next = next_free_block;
    return (void *)new_free_bloc_header;
}

// Return pointer to mem_header
mem_header * get_mem_header(){
    return (mem_header *)get_memory_adr();
}

// Return pointer of fb_head
fb * get_fb_head(){
    return ((mem_header *)get_memory_adr())->fb_head;
}

// Return pointer of current fit strategy function
mem_fit_function_t * get_curr_fit(){
    return ((mem_header *)get_memory_adr())->curr_fit;
}

//-------------------------------------------------------------
// mem_init
//-------------------------------------------------------------
void mem_init() {
    mem_fit_function_t * arg = &mem_best_fit;
    set_adress_order();

    void * memory = get_memory_adr();
    mem_header * m_head = memory;
    m_head->fb_head = init_fb_header(memory+sizeof(mem_header), get_memory_size()-sizeof(mem_header), NULL);
    mem_fit(arg);
    return;
}

//-------------------------------------------------------------
// mem_alloc
//-------------------------------------------------------------
void *mem_alloc(size_t size) {
    mem_header * m_header = get_memory_adr();
    mem_fit_function_t * curr_fit = get_curr_fit();
    fb * free_bloc_to_alloc = curr_fit(m_header->fb_head, size);
    
    // Insufficient block size
    if(free_bloc_to_alloc == NULL || size <= 0) return NULL;
    
    size_t bloc_size = free_bloc_to_alloc->taille_bloc;

    fb * tmp = m_header->fb_head;
    fb * prec = NULL;
    while(tmp != NULL && tmp != free_bloc_to_alloc){
        prec = tmp;
        tmp = tmp->next;
    }

    if(prec == NULL)
        prec = tmp;
    size_t padding_size = 0;
    // Add padding
    if((size + sizeof(al)) % PROC_ARC != 0)
        padding_size = PROC_ARC - ((size+sizeof(al)) % PROC_ARC); 
    // Check for enough place to add a Freeblock header and atleast one byte for allocation
    if((bloc_size - (size + sizeof(al) + padding_size)) <= (sizeof(fb) + 1) && ((bloc_size - (size + sizeof(al) + padding_size)) != 0)){
        padding_size = bloc_size - size - sizeof(al);
    }
    size_t offset = size + padding_size + sizeof(al);
    
    // Change fb_head if first bloc is allocated
    if(free_bloc_to_alloc == m_header->fb_head)
        m_header->fb_head = init_fb_header((void *)free_bloc_to_alloc+offset, bloc_size - offset, free_bloc_to_alloc->next);
    else
        // Add new updated free block to memory and return its address 
        prec->next = init_fb_header((void *)free_bloc_to_alloc+offset, bloc_size - offset, free_bloc_to_alloc->next);
    
    // Add extra bytes to align or extra bytes if not enough bytes for new free allocation block
    free_bloc_to_alloc->taille_bloc = offset;

    return ((void *)free_bloc_to_alloc)+sizeof(al);
}

//-------------------------------------------------------------
// mem_free
//-------------------------------------------------------------
int mem_free(void *zone) {
    // Cannot free NULL pointer
    if(zone == NULL){
        printf("NULL adr.\n");
        return 1;   
    }
    // Get pointer of zone header
    zone = address_substraction(zone, sizeof(al));
    mem_header * m_header = get_memory_adr();
    // No blocks to free
    if(m_header->fb_head != NULL && m_header->fb_head->taille_bloc == get_memory_size()-sizeof(mem_header)) return 1;
    // zone is not an allocated adress
    if(!mem_walk(NULL, zone)) return 1;

    fb * tmp;
    int flag_contiguous = 0;
    
    // Case when zone is before first free block or memory is fully allocated, need to update fb_head
    if(m_header->fb_head == NULL || compare_adresses(zone, (void *)m_header->fb_head)){
        m_header->fb_head = init_fb_header(zone, ((al *)zone)->taille_bloc, m_header->fb_head);
        tmp = m_header->fb_head;
        flag_contiguous++;
    } else {
    // Find the preceding free block to zone
        tmp = m_header->fb_head;
        fb * prec = NULL;
        while(tmp != NULL && compare_adresses(zone, ((void *) tmp)+tmp->taille_bloc)){ //(((void *) tmp)+tmp->taille_bloc > zone)
            prec = tmp;
            tmp = tmp->next;
        }

        if(tmp == NULL)
            tmp = prec;
    }

    // Left fusion
    if(((void *)tmp) + tmp->taille_bloc == zone){
        tmp = init_fb_header(tmp, tmp->taille_bloc + ((al *) zone)->taille_bloc, tmp->next );
        flag_contiguous++;
    }

    // No fusion
    if(!flag_contiguous){
        tmp->next = init_fb_header(zone, ((al *) zone)->taille_bloc, tmp->next );
        tmp = zone;
    }

    // Right fusion
    if(tmp->next != NULL){
        if(tmp->next == (((void *)tmp) + tmp->taille_bloc))
            tmp = init_fb_header(tmp, tmp->taille_bloc + tmp->next->taille_bloc, tmp->next->next);
    }
    
    return 0;
}

//-------------------------------------------------------------
// Itérateur(parcours) sur le contenu de l'allocateur
//-------------------------------------------------------------

// Walk through the current memory : either acts as mem_show (print != NULL) or checks for a valid free adress (print == NULL)
int mem_walk(void (*print)(void *, size_t, int free), void * allocd_addr){
    fb * fb_head = get_fb_head();
    void * curr_byte_addr = get_memory_adr()+sizeof(mem_header);
    fb * free_bloc_tmp = fb_head; 
    while(free_bloc_tmp != NULL || (curr_byte_addr < (get_memory_adr() + get_memory_size()))){
        if(curr_byte_addr == (void *)free_bloc_tmp){
            if(print != NULL) 
                print((void *)free_bloc_tmp, free_bloc_tmp->taille_bloc, 1);
            curr_byte_addr += free_bloc_tmp->taille_bloc;
            free_bloc_tmp = free_bloc_tmp->next;
        } else {
            al * alloc_bloc_tmp = curr_byte_addr;
            if(print == NULL){
                if(allocd_addr == curr_byte_addr)
                        return 1;
            } else
                print((void *)curr_byte_addr, alloc_bloc_tmp->taille_bloc, 0);
            curr_byte_addr += alloc_bloc_tmp->taille_bloc; 
        }
    } 
    return 0;
}

void mem_show(void (*print)(void *, size_t, int free)) {
    if(print != NULL)
        mem_walk(print, NULL);
    return;
}

//-------------------------------------------------------------
// mem_fit
//-------------------------------------------------------------
void mem_fit(mem_fit_function_t *mff) {
    mem_header * m_header = get_memory_adr();
    m_header->curr_fit = mff;
    return;
}

//-------------------------------------------------------------
// Stratégies d'allocation
//-------------------------------------------------------------
struct fb *mem_first_fit(struct fb *fb_head, size_t size) {
    fb * tmp = fb_head;
    while(tmp != NULL && tmp->taille_bloc < (size + sizeof(al))){
        tmp = tmp->next;
    }
    if(tmp != NULL) return tmp;
    return NULL;
}
//-------------------------------------------------------------
struct fb *mem_best_fit(struct fb *fb_head, size_t size) {
    if(fb_head == NULL) return NULL;
    size_t taille_min_curr = get_memory_size();
    fb * curr_min = NULL;
    fb * tmp = fb_head;
    while(tmp != NULL){
        int curr_block_diff = tmp->taille_bloc - size - sizeof(al);
        if(curr_block_diff >= 0 && curr_block_diff < taille_min_curr){
            taille_min_curr = (size_t) curr_block_diff;
            curr_min = tmp;
        }
        tmp = tmp->next;
    }
    return curr_min;
}
//-------------------------------------------------------------
struct fb *mem_worst_fit(struct fb *fb_head, size_t size) {
    if(fb_head == NULL) return NULL;
    size_t taille_max_curr = 0;
    fb * curr_max = NULL;
    fb * tmp = fb_head;
    while(tmp != NULL){
        int curr_block_diff = tmp->taille_bloc - size - sizeof(al);
        if(curr_block_diff > 0 && curr_block_diff >= taille_max_curr){
            taille_max_curr = (size_t) curr_block_diff;
            curr_max = tmp;
        }
        tmp = tmp->next;
    }
    return curr_max;
}
