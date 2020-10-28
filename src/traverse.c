#include "grammar.h"
#include "traverse.h"

#include <stdio.h>

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

void traverseParseTree(parseTree *t, typeExpressionTable *T){
	T->num_ids = 0;
	T->ids = NULL;
	create_hm(&(T->expr), 256);

	node *decl = T->root->child;

	while(decl && strcomp(decl->sym_name, "<body>")){
		decl = decl->sibling;
	}
	
	decl = decl->child; // <p_declaration>

	while(decl) {
		decl = decl->child; // <declaration>

		// TODO: Check for a <= b and INTEGER

		if(decl->child.tag == 1 && decl->child.leaf.T == EPSILON){
			break;
		}

		node *line_n = decl->child; // DECLARE
		node *ids_n = line->sibling; // <s_l_declare>
		node *type_n = line->sibling->sibling->sibling; // <type>

		type_exp *type_rec = (type_exp*) malloc(sizeof(type_exp));

		if(type_n->child->t == 1 && type_n->child->u.leaf.T == ARRAY){
			range *head = (range*) malloc(sizeof(range));
			range *ptr;

			head->next = NULL;

			type_rec->type_exp = RECTANGULAR_ARRAY;
			type_rec->rect_arr_exp.n_dim = 1;
			type_rec->rect_arr_exp.ranges = head;
			type_rec->rect_arr_exp.basic_el_type = INTEGER_T;

			node *dims = type_n->child->sibling; // SQ_OP
			dims = dims->sibling; // <idx>

			head->a = strdup(dims->child.leaf.lexeme);
			dims = dims->sibling->sibling;
			head->b = strdup(dims->child.leaf.lexeme);
			
			ptr = head;

			dims = dims->sibling->sibling; // <dims>

			while(dims){
				dims = dims->child;

				if(dims->child->t == 1 &&
						dims->child->u.leaf.T == EPSILON){
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

			type_rec->type_exp = JAGGED_ARRAY;
			type_rec->jagged_arr_exp.n_dim = 1;
			type_rec->jagged_arr_exp.x = head;
			type_rec->jagged_arr_exp.basic_el_type = INTEGER_T;

			node *dims = type_n->child->sibling->sibling; // SQ_OP
			dims = dims->sibling; // NUM

			head->a = strdup(dims.leaf.lexeme);
			dims = dims->sibling->sibling;
			head->b = strdup(dims.leaf.lexeme);

			dims = dims->sibling->sibling;
			dims = dims->sibling->sibling;

			if(dims->child.leaf.T == EPSILON)
				type_rec->jagged_arr_exp.n_dim = 2;
			else
				type_rec->jagged_arr_exp.n_dim = 3;

			dim *ptry, *heady = NULL;
			ptry = heady;

			// <jagg_inis>
			node *jagg_inis = dims->sibling->sibling->sibling->sibling;
			while(jagg_inis){
				jagged_inis = jagged_inis->child;

				if(jagg_inis.t == 1 && jagg_inis.leaf.T == EPSILON){
					break;
				}

				dim *tmp = (dim*) malloc(sizeof(dim));
				tmp->next = tmp->child = NULL;

				while(
					jagg_inis->t != 1 ||
					jagg_inis->internal.V != SIZE
				){
					jagg_inis = jagg_inis->sibling;
				}

				jagg_inis = jagg_inis->sibling;

				tmp->n = atoi(jagg_inis.leaf.lexeme);

				if(!heady){
					heady = tmp;
					ptry = heady;
				}else{
					ptry->next = tmp;
					ptry = tmp;
				}

				while(
					jagg_inis->t != 1 ||
					jagg_inis->internal.V != JAGG_ARRAY_NUMS
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
						nums->leaf.T == SEMI_C) ||
						(nums->t == 1 &&
						 nums->leaf.T == EPSILON))
					{
						num_b++;

						if(
						type_rec->jagged_arr_exp.n_dim == 2
						&&
						count >= 2)
						{
							type_rec->jagged_arr_exp.err = 1;
							print_err_d(
							nums->leaf.line_num,
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
					type_rec->jagged_arr_exp.err = 1;

					print_err_d(
						nums->leaf.line_num,
						nums->leaf->depth,
						"2D JA size mismatch"
					);
					break;
				}
				else if(
					type_rec->jagged_arr_exp.n_dim == 3
					&&
					num_b != tmp->n)
				{
					type_rec->jagged_arr_exp.err = 1;

					print_err_d(
						nums->leaf.line_num,
						nums->leaf->depth,
						"3D JA size mismatch"
					);
					break;
				}
			}
		}else{
			type_rec->type_exp = PRIMITVE;

			switch(type_n->child->u.leaf.T){
				case INTEGER:
					type_rec->rect_arr_exp.basic_el_type = INTEGER_T;
					break;
				case REAL:
					type_rec->rect_arr_exp.basic_el_type = REAL_T;
					break;
				case BOOLEAN:
					type_rec->rect_arr_exp.basic_el_type = BOOLEAN_T;
					break;
			}
		}

		if(ids_n->child->u.leaf.T == ID){
			T->num_ids++;
			insert(&(T->expr), ids_n->child->u.leaf.lexeme, type_rec);
		}else{
			T->nums_ids = 2;
			ids_n = ids_n->child;

			while(ids_n->u.leaf.T != ID){
				ids_n = ids_n->sibling;
			}

			insert(&(T->expr), ids_n->u.leaf.lexeme, type_rec);
			insert(&(T->expr), ids_n->sibling->u.leaf.lexeme, type_rec);
			ids_n = ids_n->sibling->sibling;

			while(ids_n){
				ids_n = ids_n->child;

				if(ids_n->u.leaf.T == EPSILON)
					break;

				insert(&(T->expr), ids_n->u.leaf.lexeme, type_rec);
				ids_n = ids_n->sibling;
			}
		}

		decl = decl->sibling;
	}
}
