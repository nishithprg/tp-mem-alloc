#if !defined(__MEM_H)
#define __MEM_H
#include <stddef.h>

/* -----------------------------------------------*/
/* Interface utilisateur de votre allocateur      */
/* -----------------------------------------------*/
void *mem_alloc(size_t);
int mem_free(void *);
size_t mem_get_size(void *);

/* Itérateur sur le contenu de l'allocateur */
int mem_walk(void (*print)(void *, size_t, int free), void * allocd_addr);
void mem_show(void (*print)(void *, size_t, int free));

#endif
