#include "grammar.h"
#include "parser.h"
#include "traverse.h"
#include "typeexp.h"

#include <stdio.h>
#include <ctype.h>


void print_err_d(int line_num,
		int depth,
		char *message)
{
	printf("\nERROR>> %12d %20s %s %s %s %s %s %12d %30s\n",
		line_num, "declaration", "***", "***", "***",
		"***", "***", depth, message);
}

void printTypeExpressionTable(typeExpressionTable *T){
	id_list *ptr = T->ids;

	printf("\nType Expression Table\n");
	printf("%20s\tField 2\t Field 3\tField 4\n", "Field 1");

	while(ptr){
		char texp_s[1000] = "<";

		type_exp *texp = (type_exp*) get(&(T->expr), ptr->id);
		if(texp->texp_type == PRIMITIVE){
			if(texp->prim_exp.basic_el_type == INTEGER_T){
				printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, "NA");
				strcat(texp_s, "type=integer>");
			}else if(texp->prim_exp.basic_el_type == REAL_T){
				printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, "NA");
				strcat(texp_s, "type=real>");
			}else if(texp->prim_exp.basic_el_type == BOOLEAN_T){
				printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, "NA");
				strcat(texp_s, "type=boolean>");
			}
		}else if(texp->err == 1){
			strcpy(texp_s, "***");
			if(texp->texp_type == RECTANGULAR_ARRAY)
				printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, "***");
			else
				printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, "NA");
		}else if(texp->texp_type == RECTANGULAR_ARRAY){
			strcat(texp_s, "type=rectangularArray, dimensions=");
			int dynamic = 0;
			int dim = 0;
			
			char rem[1000] = "";
			range *rptr = texp->rect_arr_exp.ranges;
			while(rptr){
				dim++;
				sprintf(rem, "%s, range_R%d=(%s,%s)", rem, dim, rptr->a, rptr->b);
				if(isdigit((rptr->a)[0]) == 0 || isdigit((rptr->b)[0]) == 0){
					dynamic = 1;
				}

				rptr = rptr->next;
			}

			char dim_s[32];
			sprintf(dim_s, "%d", dim);
			strcat(texp_s, dim_s);
			strcat(texp_s, rem);
			strcat(texp_s, ", basicElementType=integer>");
			printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, dynamic ? "dynamic" : "static");
		}else if(texp->texp_type == JAGGED_ARRAY){
			strcat(texp_s, "type=jaggedArray, dimensions=");

			char rem[1000] = "";
			if(texp->jagged_arr_exp.n_dim == 2){
				sprintf(rem, "2, range_R1=(%s, %s), range_R2=(", texp->jagged_arr_exp.x->a, texp->jagged_arr_exp.x->b);

				dim *dptr = texp->jagged_arr_exp.y;

				sprintf(rem, "%s%d", rem, dptr->n);

				dptr = dptr->next;

				while(dptr){
					sprintf(rem, "%s, %d", rem, dptr->n);
					dptr = dptr->next;
				}

			}else{
				sprintf(rem, "3, range_R1=(%s, %s), range_R2=(", texp->jagged_arr_exp.x->a, texp->jagged_arr_exp.x->b);

				dim *dptr = texp->jagged_arr_exp.y;

				sprintf(rem, "%s%d", rem, dptr->n);

				dim *cptr = dptr->child;
				if(cptr){
					sprintf(rem, "%s [%d", rem, cptr->n);
					cptr = cptr->next;

					while(cptr){
						sprintf(rem, "%s, %d", rem, cptr->n);
						cptr = cptr->next;
					}

					strcat(rem, "]");
				}
				dptr = dptr->next;

				while(dptr){
					sprintf(rem, "%s, %d", rem, dptr->n);
					
					cptr = dptr->child;
					if(cptr){
						sprintf(rem, "%s [%d", rem, cptr->n);
						cptr = cptr->next;

						while(cptr){
							sprintf(rem, "%s, %d", rem, cptr->n);
							cptr = cptr->next;
						}

						strcat(rem, "]");
					}

					dptr = dptr->next;
				}
			}

			strcat(texp_s, rem);
			strcat(texp_s, "), basicElementType=integer>");
			printf("%20s\t%d\t%8s\t", ptr->id, texp->texp_type, "NA");
		}

		printf("%s\n", texp_s);

		ptr = ptr->next;
	}
}

void traverseParseTree(node *t, typeExpressionTable *T){
	T->num_ids = 0;
	T->ids = NULL;
	create_hm(&(T->expr), 256);

	node *decl = t->child;
	
	while(decl && (decl->t != 0 || decl->u.internal.V != BODY)){	
		decl = decl->sibling;
	}	

	decl = decl->child; // <p_declaration>
	int count = 0;

	while(decl) {
		/*
		printf("Tag: %d, T: %d, V: %d, leaf.lexeme: %s\n", decl->t,
				decl->u.leaf.T,
				decl->u.internal.V,
				decl->u.leaf.lexeme);
			*/
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

			node *anode = dims->child;
			head->a = strdup(dims->child->u.leaf.lexeme);
			dims = dims->sibling->sibling;
			head->b = strdup(dims->child->u.leaf.lexeme);

			if(isdigit((head->a)[0]) == 0){
				if(has_key(&(T->expr), head->a) == 0){
					type_rec->err = 1;
					print_err_d(
						anode->u.leaf.line_num,
						anode->depth,
						"Rect Array dimension invalid"
					);
				}

				type_exp *texp = (type_exp*) get(
						&(T->expr),
						head->a);

				if(texp->texp_type != PRIMITIVE ||
					texp->prim_exp.basic_el_type != INTEGER_T){
					type_rec->err = 1;
					print_err_d(
						anode->u.leaf.line_num,
						anode->depth,
						"Rect Array dimension invalid"
					);
				}
			}

			if(isdigit((head->b)[0]) == 0){
				if(has_key(&(T->expr), head->b) == 0){
					type_rec->err = 1;
					print_err_d(
						dims->child->u.leaf.line_num,
						dims->child->depth,
						"Rect Array dimension invalid"
					);
				}

				type_exp *texp = (type_exp*) get(
						&(T->expr),
						head->b);

				if(texp->texp_type != PRIMITIVE ||
					texp->prim_exp.basic_el_type != INTEGER_T){
					type_rec->err = 1;
					print_err_d(
						dims->child->u.leaf.line_num,
						dims->child->depth,
						"Rect Array dimension invalid"
					);
				}
			}

			ptr = head;

			dims = dims->sibling->sibling; // <dims>

			while(dims){
				dims = dims->child;
				
				if(dims->t == 1 &&
						dims->u.leaf.T == EPSILON){
					break;
				}

				type_rec->rect_arr_exp.n_dim++;

				range *tmp = (range*) malloc(sizeof(range));
				ptr->next = tmp;
				tmp->next = NULL;

				dims = dims->sibling; // <idx>

				anode = dims->child;
				tmp->a = strdup(dims->child->u.leaf.lexeme);
				dims = dims->sibling->sibling; // <idx>
				tmp->b = strdup(dims->child->u.leaf.lexeme);


				if(isdigit((tmp->a)[0]) == 0){
					if(has_key(&(T->expr), tmp->a) == 0){
						type_rec->err = 1;
						print_err_d(
							anode->u.leaf.line_num,
							anode->depth,
							"Rect Array dimension invalid"
						);
					}

					type_exp *texp = (type_exp*) get(
							&(T->expr),
							tmp->a);

					if(texp->texp_type != PRIMITIVE ||
						texp->prim_exp.basic_el_type != INTEGER_T){
						type_rec->err = 1;
						print_err_d(
							anode->u.leaf.line_num,
							anode->depth,
							"Rect Array dimension invalid"
						);
					}
				}

				if(isdigit((tmp->b)[0]) == 0){
					if(has_key(&(T->expr), tmp->b) == 0){
						type_rec->err = 1;
						print_err_d(
							dims->child->u.leaf.line_num,
							dims->child->depth,
							"Rect Array dimension invalid"
						);
					}

					type_exp *texp = (type_exp*) get(
							&(T->expr),
							tmp->b);

					if(texp->texp_type != PRIMITIVE ||
						texp->prim_exp.basic_el_type != INTEGER_T){
						type_rec->err = 1;
						print_err_d(
							dims->child->u.leaf.line_num,
							dims->child->depth,
							"Rect Array dimension invalid"
						);
					}
				}
				
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
				node *R1 = jagg_inis;

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
					type_rec->jagged_arr_exp.y = heady;
				}else{
					ptry->next = tmp;
					ptry = tmp;
				}
	
				while(
					jagg_inis->t != 0 ||
					jagg_inis->u.internal.V != JAGG_ARRAY_NUMS
				){
					jagg_inis = jagg_inis->sibling;
				}

				int count = 0;
				int num_b = 0;
				dim *headz = NULL;
				dim *ptrz = headz;

				int reported = 0;

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
							R1->u.leaf.line_num,
							R1->depth,
							"2D JA size mismatch"
							);
							reported = 1;
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

					if(!reported)
						print_err_d(
							R1->u.leaf.line_num,
							R1->depth,
							"2D JA size mismatch"
						);
				}
				else if(
					type_rec->jagged_arr_exp.n_dim == 3
					&&
					num_b != tmp->n)
				{
					type_rec->err = 1;

					print_err_d(
						R1->u.leaf.line_num,
						R1->depth,
						"3D JA size mismatch"
					);
				}
				jagg_inis = jagg_inis->sibling->sibling;
			}
		}else{
			type_rec->texp_type = PRIMITIVE;

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

	id_list *ptr = T->ids;
	id_list *prev = NULL;

	while(ptr){
		id_list *tmp = ptr->next;
		ptr->next = prev;
		prev = ptr;
		ptr = tmp;
	}

	T->ids = prev;

	printTypeExpressionTable(T);
}
