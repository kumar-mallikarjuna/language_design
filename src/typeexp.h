#include "grammar.h"

#include <stdlib.h>
#ifndef TYPEEXP_H
#define TYPEEXP_H

typedef enum {
    PRIMITVE,
    RECTANGULAR_ARRAY,
    JAGGED_ARRAY,
} type;

typedef enum{
    INTEGER_T,
    REAL_T,
    BOOLEAN_T
}basic_el;

typedef struct dim_t {
	int n;
	struct dim_t *next;
	struct dim_t *child;
} dim;

typedef struct range_t {
	char *a;
	char *b;
	struct range_t *next;
} range;

typedef struct _primitive_type_exp{
	basic_el basic_el_type;
}primitive_type_exp;

typedef struct _rect_arr_type_exp{
    int n_dim; 
    range *ranges;
    basic_el basic_el_type;
}rect_arr_type_exp;

typedef struct _jagged_arr_type_exp{
    int n_dim; 
    range *x;
    dim *y;
    basic_el basic_el_type;
}jagged_arr_type_exp;

typedef struct _type_exp_entry{
    int err;
    type texp_type;
    union{
        primitive_type_exp prim_exp;
        rect_arr_type_exp rect_arr_exp;
        jagged_arr_type_exp jagged_arr_exp;
    };
}type_exp;

/*ignore, not using*/
//making use of the "struct hack" in C99
//during allocation, need to make space for size of struct + space for the arrays:
//jagged_arr_type_exp *t = malloc(sizeof(jagged_arr_type_exp) + 100);

#endif
