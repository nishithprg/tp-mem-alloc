#if !defined(mem_os_h)
#define mem_os_h

typedef struct fb fb;

struct fb {
    size_t taille_bloc;
    fb * next;
};

typedef struct al al;

struct al {
    size_t taille_bloc;
};


/* -----------------------------------------------*/
/* Interface de gestion de votre allocateur       */
/* -----------------------------------------------*/
// Initialisation
void mem_init(void);

// Définition du type mem_fit_function_t
// type des fonctions d'allocation
typedef struct fb *(mem_fit_function_t)(struct fb *, size_t);

// Declarations of mem_header and mem_header as type
struct mem_header {
    fb* fb_head;
    mem_fit_function_t *curr_fit;
};

typedef struct mem_header mem_header;

// Choix de la fonction d'allocation
// = choix de la stratégie de l'allocation
void mem_fit(mem_fit_function_t *);

// Stratégies de base (fonctions) d'allocation
mem_fit_function_t mem_first_fit;
mem_fit_function_t mem_worst_fit;
mem_fit_function_t mem_best_fit;

#endif /* mem_os_h */
