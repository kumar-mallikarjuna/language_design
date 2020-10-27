#include<stdlib.h>
#ifndef TYPEEXP_H
#define TYPEEXP_H

typedef enum {
    INTEGER,
    REAL,
    BOOLEAN,
    RECTANGULAR_ARRAY,
    JAGGED_ARRAY,
} type;
typedef struct _type_exp_entry{
    type texp_type;
    union{
        primitive_type_exp prim_exp;
        rect_arr_type_exp rect_arr_exp;
        jagged_arr_type_exp jagged_arr_exp;
    };
}type_exp;

typedef enum{
    INTEGER
}basic_el;


typedef struct _primitive_type_exp{

}primitive_type_exp;

typedef struct _rect_arr_type_exp{
    int dim; 
    int **r1;
    int **r2;
    basic_el basic_el_type;
}rect_arr_type_exp;

typedef struct _jagged_arr_type_exp{
    int dim; 
    int **r1;
    int **r2;
    basic_el basic_el_type;
}jagged_arr_type_exp;

/*ignore, not using*
//making use of the "struct hack" in C99
//during allocation, need to make space for size of struct + space for the arrays:
//jagged_arr_type_exp *t = malloc(sizeof(jagged_arr_type_exp) + 100);

#endif