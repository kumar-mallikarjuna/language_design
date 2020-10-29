//line 37 strcomp
void arith_type(node* t, errorlist err){ //fix error list
	if(t == NULL) return;

	if((strcmp(t -> sym_name, "ADDOP")==0 )||(strcmp(t -> sym_name, "MULOP")==0 )){
		node* childd = t->child;
		node* t1,t2;
		t1 = childd -> sibling; t2 = t1 -> sibling;
		term op = childd->leaf.T; // FIX condition

		arith_type(t1, err);
		arith_type(t2 , err);

		if(t1->nodeErr ==1 || t2->nodeErr ==1) { //1 denotes error
			t ->nodeErr = 1;
			return;
		}
		if((t1->t ==1 && t1->leaf.T == INTEGER) && (t2->t ==1 && t2->leaf.T == INTEGER) 
			&& (op = MULOP || op == ADDOP)){
				t -> nodeErr = 0 ; //type of node
				return;
			}
		
		if((t1->t ==1 && t1->leaf.T == REAL) && (t2->t ==1 && t2->leaf.T == REAL) 
			&& (op = MULOP || op == ADDOP)){
				t -> nodeErr = 0 ;
				return;
	
			}

		if((t1->t ==1 && t1->leaf.T == INTEGER) && (t2->t ==1 && t2->leaf.T == REAL) 
			&& (op = MULOP || op == ADDOP)){
				t -> nodeErr = 0 ; //type of node
				return;
			}

		if((t1->t ==1 && t1->leaf.T == REAL) && (t2->t ==1 && t2->leaf.T == INTEGER) 
			&& (op = MULOP || op == ADDOP)){
				t -> nodeErr = 0 ; //type of node
				return;
			}
			
	}
	//print error message
		return;
	
}

void bool_type(node* t, errorlist err){
	if(t == NULL) return;
}


void assn_traverse(parseTree *t, typeExpressionTable *T){
	node* assn; //init?

	while (assn != NULL){
		if(strcmp(assn->sym_name, "<body>")==0){
			break;
		}
		assn = assn-> sibling;
	}
	//body -> p_assignment
	assn = assn -> child->sibling; 
	//p_assignment -> assignment
	assn = assn -> child; 

	while( assn != NULL){
		//int check = dfs(assn);
		//if(check == 0 ){//type mismatch
		//}
	}

}

/************************* 2nd implmentation *****************/
//FOR ADDOP after calling this we do for assignopx
void traverseParseTree(parseTree *t, typeExpressionTable T ){
	if(t==NULL){
		return;
	}
	if(t->sym_name == "ADDOP" || t->sym_name == "MULOP" || t->sym_name == "B_OR" || t->sym_name == "B_AND"){

		node * operand2 = t->sibling;
		node * operand1;

		node* parent = t->parent;
		node* operand1 = parent->child;

		while(operand1->sibling != t){
			operand1 = operand1->sibling;
		}

		if(operand1->sym_name == "SQ_CL"){
			operand1 = parent->child;
			node * temp1 = operand1;
			node* temp2 = operand2;	
			int termCount = 1;
			while(operand1->sibling != t){
				if(operand1->t == 0){
					termCount++;
				}
				operand1 = operand1->sibling;
			}
			
			operand1 = parent->child;

			while(true){
				if(operand1->t == 0){
					termCount--;
					if(termCount == 0){
						break;
					}
				}
				operand1 = operand1->sibling;
			}
		}

		if(operand1->child->t == 0){
			traverseParseTree(operand1->child->t, T);
			//TypeE of operand1->child = opernad1
		}
		else{
			//Initialize type of operand1->child using table and make it as operand1's type
		}

		if(operand2->child->t == 0){
			traverseParseTree(operand2->child->t, T);
			//TypeE of operand2->child = opernad1
		}
		else{
			//Initialize type of operand2->child using table and make it as operand2's type
		}
		//Now check if typeE of operand1==operand2, if it is the no error, make typeE = ADDOP ka typeE, else error.

	}
	recurseTree(t->child);
	recurseTree(t->sibling);

}