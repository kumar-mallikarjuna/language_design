#include "grammar.h"
#include "parser.h"
#include "traverse.h"
#include "typeexp.h"

#include <stdio.h>
#include <ctype.h>

void print_err_a(int line_num,
		int statement_type,
		non_term operator,
		char *lexeme1,
		non_term type1,
		char *lexeme2,
		non_term type2,
		int depth,
		char *message){
	printf("%12d %20s %20s %20s %20s %20s %20s %12d %30s",
		line_num, "declaration", "***", "***", "***",
		"***", "***", depth, message);
}


void print_err_d(int line_num,
		int depth,
		char *message)
{
	printf("%12d %20s %20s %20s %20s %20s %20s %12d %30s",
		line_num, "declaration", "***", "***", "***",
		"***", "***", depth, message);
}

void printTypeExpressionTable(typeExpressionTable *T){
	id_list *ptr = T->ids;

	char texp_s[1000] = "<";

	while(ptr){
		type_exp *texp = (type_exp*) get(&(T->expr), ptr->id);
		if(texp->texp_type == PRIMITVE){
			if(texp->prim_exp.basic_el_type == INTEGER_T)
				printf("%20s\t%d\t%6s\t\n", ptr->id, texp->texp_type, "NA");
			else if(texp->prim_exp.basic_el_type == REAL_T)
				printf("%20s\t%d\t%6s\t\n", ptr->id, texp->texp_type, "NA");
			else if(texp->prim_exp.basic_el_type == BOOLEAN_T)
				printf("%20s\t%d\t%6s\t\n", ptr->id, texp->texp_type, "NA");
		}else if(texp->texp_type == RECTANGULAR_ARRAY){
			int dynamic = 0;

			range *rptr = texp->rect_arr_exp.ranges;
			while(rptr){
				if(isdigit((rptr->a)[0]) == 0 || isdigit((rptr->b)[0]) == 0){
					dynamic = 1;
					break;
				}

				rptr = rptr->next;
			}

			printf("%20s\t%d\t%6s\t\n", ptr->id, texp->texp_type, dynamic ? "dynamic" : "static");
		}else if(texp->texp_type == JAGGED_ARRAY){
			printf("%20s\t%d\t%6s\t\n", ptr->id, texp->texp_type, "NA");
		}

		ptr = ptr->next;
	}
}

void traverseParseTree(node *t, typeExpressionTable *T){
	T->num_ids = 0;
	T->ids = NULL;
	create_hm(&(T->expr), 256);

	node *decl = t->child;
	
	printf("\n");

	while(decl && (decl->t != 0 || decl->u.internal.V != BODY)){	
		decl = decl->sibling;
	}	

	decl = decl->child; // <p_declaration>
	int count = 0;

	while(decl) {
		printf("Tag: %d, T: %d, V: %d, leaf.lexeme: %s\n", decl->t,
				decl->u.leaf.T,
				decl->u.internal.V,
				decl->u.leaf.lexeme);
		if(count == 0){
			decl = decl->child; // <declaration>	
		}

		// TODO: Check for a <= b and INTEGER
		
		if(decl->child->t == 1 && decl->child->u.leaf.T == EPSILON){
			break;
		}

		node *line_n = decl->child; // DECLARE
		node *ids_n = line_n->sibling; // <s_l_declare>
		node *type_n = line_n->sibling->sibling->sibling; // <type>

		type_exp *type_rec = (type_exp*) malloc(sizeof(type_exp));

		if(type_n->child->t == 1 && type_n->child->u.leaf.T == ARRAY){
			range *head = (range*) malloc(sizeof(range));
			range *ptr;

			head->next = NULL;

			type_rec->texp_type = RECTANGULAR_ARRAY;
			type_rec->rect_arr_exp.n_dim = 1;
			type_rec->rect_arr_exp.ranges = head;
			type_rec->rect_arr_exp.basic_el_type = INTEGER_T;

			node *dims = type_n->child->sibling; // SQ_OP
			dims = dims->sibling; // <idx>

			head->a = strdup(dims->child->u.leaf.lexeme);
			dims = dims->sibling->sibling;
			head->b = strdup(dims->child->u.leaf.lexeme);
			
			ptr = head;

			dims = dims->sibling->sibling; // <dims>

			while(dims){
				dims = dims->child;
				
				printf("\tTag: %d, T: %d, V: %d, leaf.lexeme: %s\n", dims->t,
					dims->u.leaf.T,
					dims->u.internal.V,
					dims->u.leaf.lexeme);
				if(dims->t == 1 &&
						dims->u.leaf.T == EPSILON){
					break;
				}

				type_rec->rect_arr_exp.n_dim++;

				range *tmp = (range*) malloc(sizeof(range));
				ptr->next = tmp;
				tmp->next = NULL;

				dims = dims->sibling; // <idx>

				tmp->a = strdup(dims->child->u.leaf.lexeme);
				dims = dims->sibling->sibling; // <idx>
				tmp->b = strdup(dims->child->u.leaf.lexeme);
				
				ptr = tmp;

				dims = dims->sibling->sibling; // <dims>
			}
		}else if(type_n->child->t == 1 && type_n->child->u.leaf.T == JAGGED){
			range *head = (range*) malloc(sizeof(range));

			head->next = NULL;

			type_rec->texp_type = JAGGED_ARRAY;
			type_rec->jagged_arr_exp.n_dim = 1;
			type_rec->jagged_arr_exp.x = head;
			type_rec->jagged_arr_exp.basic_el_type = INTEGER_T;

			node *dims = type_n->child->sibling->sibling; // SQ_OP
			dims = dims->sibling; // NUM

			head->a = strdup(dims->u.leaf.lexeme);
			dims = dims->sibling->sibling;
			head->b = strdup(dims->u.leaf.lexeme);

			dims = dims->sibling->sibling;
			dims = dims->sibling->sibling;

			if(dims->child->u.leaf.T == EPSILON)
				type_rec->jagged_arr_exp.n_dim = 2;
			else
				type_rec->jagged_arr_exp.n_dim = 3;

			dim *ptry, *heady = NULL;
			ptry = heady;

			// <jagg_inis>
			node *jagg_inis = dims->sibling->sibling->sibling->sibling;
			while(jagg_inis){
				jagg_inis = jagg_inis->child;

				if(jagg_inis->t == 1 && jagg_inis->u.leaf.T == EPSILON){
					break;
				}

				dim *tmp = (dim*) malloc(sizeof(dim));
				tmp->next = tmp->child = NULL;

				while(
					jagg_inis->t != 1 ||
					jagg_inis->u.internal.V != SIZE
				){
					jagg_inis = jagg_inis->sibling;
				}

				jagg_inis = jagg_inis->sibling;

				tmp->n = atoi(jagg_inis->u.leaf.lexeme);

				if(!heady){
					heady = tmp;
					ptry = heady;
				}else{
					ptry->next = tmp;
					ptry = tmp;
				}

				while(
					jagg_inis->t != 1 ||
					jagg_inis->u.internal.V != JAGG_ARRAY_NUMS
				){
					jagg_inis = jagg_inis->sibling;
				}

				int count = 0;
				int num_b = 0;
				dim *headz = NULL;
				dim *ptrz = headz;

				node *nums = jagg_inis;
				while(nums){
					nums = nums->child;

					if((nums->t == 1 &&
						nums->u.leaf.T == SEMI_C) ||
						(nums->t == 1 &&
						 nums->u.leaf.T == EPSILON))
					{
						num_b++;

						if(
						type_rec->jagged_arr_exp.n_dim == 2
						&&
						count >= 2)
						{
							type_rec->err = 1;
							print_err_d(
							nums->u.leaf.line_num,
							-1,
							"2D JA size mismatch"
							);
							break;
						}

						if(
						type_rec->jagged_arr_exp.n_dim == 3
						){
							dim *tmpz = (dim*) malloc(sizeof(headz));

							tmpz->n = count;

							if(!headz){
								headz = tmpz;
								ptrz = headz;
								tmp->child = headz;
							}else{
								ptrz->next = tmpz;
								ptrz = tmpz;
							}
						}

						count = -1;
					}

					count++;
					nums = nums->sibling;
				}

				if(
					type_rec->jagged_arr_exp.n_dim == 2
					&&
					num_b != tmp->n)
				{
					type_rec->err = 1;

					print_err_d(
						nums->u.leaf.line_num,
						nums->depth,
						"2D JA size mismatch"
					);
					break;
				}
				else if(
					type_rec->jagged_arr_exp.n_dim == 3
					&&
					num_b != tmp->n)
				{
					type_rec->err = 1;

					print_err_d(
						nums->u.leaf.line_num,
						nums->depth,
						"3D JA size mismatch"
					);
					break;
				}
			}
		}else{
			type_rec->texp_type = PRIMITVE;

			if(type_n->child->u.leaf.T == INTEGER)
				type_rec->prim_exp.basic_el_type = INTEGER_T;
			else if(type_n->child->u.leaf.T == REAL)
				type_rec->prim_exp.basic_el_type = REAL_T;
			else if(type_n->child->u.leaf.T == BOOLEAN)
				type_rec->prim_exp.basic_el_type = BOOLEAN_T;
		}

		if(ids_n->child->u.leaf.T == ID){
			//printf("%s\n", ids_n->child->u.leaf.lexeme);
			id_list *idx = (id_list*) malloc(sizeof(id_list));
			strcpy(idx->id, ids_n->child->u.leaf.lexeme);
			idx->next = T->ids;
			T->ids = idx;
			T->num_ids++;

			insert(&(T->expr), ids_n->child->u.leaf.lexeme, type_rec);
		}else{
			T->num_ids += 2;
			ids_n = ids_n->child;

			while(ids_n->u.leaf.T != ID){
				ids_n = ids_n->sibling;
			}

			//printf("%s\n", ids_n->u.leaf.lexeme);
			id_list *idx = (id_list*) malloc(sizeof(id_list));
			strcpy(idx->id, ids_n->u.leaf.lexeme);
			idx->next = T->ids;
			T->ids = idx;
			insert(&(T->expr), ids_n->u.leaf.lexeme, type_rec);

			//printf("%s\n", ids_n->sibling->u.leaf.lexeme);
			idx = (id_list*) malloc(sizeof(id_list));
			strcpy(idx->id, ids_n->sibling->u.leaf.lexeme);
			idx->next = T->ids;
			T->ids = idx;
			insert(&(T->expr), ids_n->sibling->u.leaf.lexeme, type_rec);
			ids_n = ids_n->sibling->sibling;

			while(ids_n){
				ids_n = ids_n->child;

				if(ids_n->u.leaf.T == EPSILON)
					break;

				//printf("%s\n", ids_n->u.leaf.lexeme);
				id_list *idx = (id_list*) malloc(sizeof(id_list));
				strcpy(idx->id, ids_n->u.leaf.lexeme);
				idx->next = T->ids;
				T->ids = idx;
				insert(&(T->expr), ids_n->u.leaf.lexeme, type_rec);
				ids_n = ids_n->sibling;
			}
		}

		if(count == 0){
			decl = decl->sibling;
			count = 1;
		}else{
			decl = type_n->sibling;
		}

		/*
		printf("Tag: %d, T: %d, V: %d, leaf.lexeme: %s\n", decl->t,
				decl->u.leaf.T,
				decl->u.internal.V,
				decl->u.leaf.lexeme);
				*/
	}

	printTypeExpressionTable(T);
}
