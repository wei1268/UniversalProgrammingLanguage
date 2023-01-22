#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <limits.h>
#include "String.c"
#include "Hashtable.c"

#define lit(x) x->annotation->s->str

int line_no = 1;

hashtable_t *types;

void* abort_on_error_malloc(int amt){
    void* returning = malloc(amt);
    if(!returning){
        fprintf(stderr, "Out of Memory!");
        abort();
    }
    return returning;
}

void error(){
    fprintf(stderr, "\nError\n");
    abort();
}

struct Anno{
  String s;
  int type;
};

typedef struct Anno Annotation;

Annotation* get_annotation(){
    Annotation* returning = abort_on_error_malloc(sizeof(Annotation));
    return returning;
}

/* ### Structure Definitions Start ### */
struct out_ast
{
int type;
	Annotation* annotation;
    int nl;
	struct generic_ast* to_print;
};

struct procedure_call_ast
{
int type;
	Annotation* annotation;
	char* name;
};

struct generic_ast
{
	int type;
	Annotation* annotation;
};

struct value_ast
{
int type;
	Annotation* annotation;
	int data_type;
	char* data;
};

struct decl_ast
{
int type;
	Annotation* annotation;
	char* name;
	struct generic_ast* becomes;
};

struct fork_ast
{
int type;
	Annotation* annotation;
	int op;
	struct generic_ast* left;
	struct generic_ast* right;
};

struct if_ast
{
int type;
	Annotation* annotation;
	struct generic_ast* cond;
	struct generic_ast* then;
	struct generic_ast* else_then;
};

struct while_ast
{
int type;
	Annotation* annotation;
	struct generic_ast* cond;
	struct generic_ast* then;
};

struct procedure_ast
{
int type;
    Annotation* annotation;
    struct generic_ast* to_run;
    char* name;
};

struct generic_list_ast
{
int type;
	Annotation* annotation;
	struct generic_ast* next;
	struct generic_ast* current;
};
/* ### Structure Definitions End ### */


/* ### UTILITY START ### */
struct ast_list{
    struct ast_list*  next;
    struct generic_ast* current;
};

struct ast_list* true_head_def;
struct ast_list* curr_head_def;

struct ast_list* true_head_proc;
struct ast_list* current_head_proc;

/* for definitions of variables */
void def_add(struct generic_ast* to_add){
    curr_head_def->current = to_add;
    curr_head_def->next = abort_on_error_malloc(sizeof(struct ast_list));
    curr_head_def = curr_head_def->next;
    curr_head_def->next = ((struct ast_list*) NULL);
    curr_head_def->current = ((struct generic_ast*) NULL);
}

/* for definitions of functions */
void dcl_add(struct generic_ast* to_add){
    current_head_proc->current = to_add;
    current_head_proc->next = abort_on_error_malloc(sizeof(struct ast_list));
    current_head_proc = current_head_proc->next;
    current_head_proc->next = ((struct ast_list*) NULL);
    current_head_proc->current = ((struct generic_ast*) NULL);
}
/* ### UTILITY END ### */


/* ### Forward Reference Start ### */
struct generic_ast* create_ast_fork(int op, struct generic_ast* left, struct generic_ast* right);

struct generic_ast* create_ast_out(struct generic_ast* to_print, int nl);

void destruct_ast_out(struct out_ast* to_destroy);

void evaluate_ast_out(struct out_ast* this);

struct generic_ast* create_ast_procedure_call(char* name);

void destruct_ast_procedure_call(struct procedure_call_ast* to_destroy);

void evaluate_ast_procedure_call(struct procedure_call_ast* this);

void destruct_ast_fork(struct fork_ast* to_destroy);

void evaluate_ast_fork(struct fork_ast* this);

struct generic_ast* create_ast_value(int data_type, char* data);

void destruct_ast_value(struct value_ast* to_destroy);

void evaluate_ast_value(struct value_ast* this);

struct generic_ast* create_ast_decl(char* name, struct generic_ast* becomes);

void destruct_ast_decl(struct decl_ast* to_destroy);

void evaluate_ast_decl(struct decl_ast* this);

void destruct_generic_ast(struct generic_ast* to_destroy);

struct generic_ast* create_ast_if(struct generic_ast* cond, struct generic_ast* then, struct generic_ast* else_then);

void destruct_ast_if(struct if_ast* to_destroy);

void evaluate_ast_if(struct if_ast* this);

struct generic_ast* create_ast_while(struct generic_ast* cond, struct generic_ast* then);

void destruct_ast_while(struct while_ast* to_destroy);

void evaluate_ast_while(struct while_ast* this);

struct generic_ast* create_ast_procedure(struct generic_ast* to_run, char* name);

void destruct_ast_procedure(struct procedure_ast* to_destroy);

void evaluate_ast_procedure(struct procedure_ast* this);

struct generic_ast* create_ast_generic_list(struct generic_ast* next, struct generic_ast* current);

void destruct_ast_generic_list(struct generic_list_ast* to_destroy);

void evaluate_ast_generic_list(struct generic_list_ast* this);

void destruct_generic_ast(struct generic_ast* to_destroy);

void evaluate(struct generic_ast* to_evaluate);
/* ### Forward Reference End ### */



/* ### Function Syntax Start ### */
void evaluate(struct generic_ast* to_evaluate)
{
    to_evaluate->annotation = get_annotation();
	switch(to_evaluate->type)
	{
	case 0:
		return evaluate_ast_fork((struct fork_ast*) to_evaluate);
		break;
	case 1:
		return evaluate_ast_if((struct if_ast*) to_evaluate);
		break;
	case 2:
		return evaluate_ast_while((struct while_ast*) to_evaluate);
		break;
	case 3:
		return evaluate_ast_procedure((struct procedure_ast*) to_evaluate);
		break;
	case 4:
		return evaluate_ast_generic_list((struct generic_list_ast*) to_evaluate);
		break;
    case 5:
        return evaluate_ast_value((struct value_ast*) to_evaluate);
        break;
    case 6:
        return evaluate_ast_decl((struct decl_ast*) to_evaluate);
        break;
    case 7:
        evaluate_ast_out((struct out_ast*) to_evaluate);
        break;
    case 8:
        evaluate_ast_procedure_call((struct procedure_call_ast*) to_evaluate);
        break;
	}
}

void evaluate_ast_out(struct out_ast* this)
{
	evaluate(this->to_print);
    switch(this->to_print->annotation->type){
        case 0:
            this->annotation->s = combine(3, "printf(\"%d\", ", this->to_print->annotation->s->str, ");");
            break;
        case 1:
            this->annotation->s = combine(3, "printf(\"%d\", ", this->to_print->annotation->s->str, ");");
            break;
        case 2:
            this->annotation->s = combine(3, "printf(\"%lf\", ", this->to_print->annotation->s->str, ");");
            break;
    }
    if(this->nl){ /* memory leak here */
        this->annotation->s = append(this->annotation->s, string_like("printf(\"\\n\");"));
    }
}

void evaluate_ast_procedure_call(struct procedure_call_ast* this)
{
	this->annotation->s = combine(2, this->name, "();");
}

void evaluate_ast_fork(struct fork_ast* this)
{
    // printf("HERE187\n");
	evaluate(this->right);
    evaluate(this->left);
    String to_del;

    int type = 1;

    // printf("HERE100\n");
    fflush(stdout);
    switch(this->right->annotation->type){
        case 0:
            to_del = this->left->annotation->s;
            this->left->annotation->s = append(string_like("(int)"), to_del);
            break;
        case 1:
            to_del = this->right->annotation->s;
            this->right->annotation->s = append(string_like("(int)"), to_del);
            break;
        case 2:
            to_del = this->right->annotation->s;
            this->right->annotation->s = append(string_like_burn("(double)"), to_del);
            type = 2;
            break;
    }
    // printf("HERE199\n");
    //delete(to_del);

    // printf("HERE200\n");

    switch(this->left->annotation->type){
        case 0:
            to_del = this->left->annotation->s;
            this->left->annotation->s = append(string_like_burn("(int)"), to_del);
            break;
        case 1:
            to_del = this->left->annotation->s;
            this->left->annotation->s = append(string_like_burn("(int)"), to_del);
            break;
        case 2:
            to_del = this->left->annotation->s;
            this->left->annotation->s = append(string_like_burn("(double)"), to_del);
            type = 2;
            break;
    }
    //delete(to_del);

    switch(this->op){
        case 1:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "+", "(", lit(this->right), "))");
            this->annotation->type = type;
            break;
        case 2:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "-", "(", lit(this->right), "))");
            this->annotation->type = type;
            break;
        case 3:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "*", "(", lit(this->right), "))");
            this->annotation->type = type;
            break;
        case 4:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "/", "(", lit(this->right), "))");
            this->annotation->type = type;
            break;
        case 5:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "%", "(", lit(this->right), "))");
            this->annotation->type = 1;
            break;
        case 6:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "<", "(", lit(this->right), "))");
            this->annotation->type = 0;
            break;
        case 7:
            this->annotation->s = combine(7, "((", lit(this->left), ")", ">", "(", lit(this->right), "))");
            this->annotation->type = 0;
            break;
        case 8:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "<=", "(", lit(this->right), "))");
            this->annotation->type = 0;
            break;
        case 9:
            this->annotation->s = combine(7, "((", lit(this->left), ")", ">=", "(", lit(this->right), "))");
            this->annotation->type = 0;
            break;
        case 10:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "==", "(", lit(this->right), "))");
            this->annotation->type = 0;
            break;
        case 11:
            this->annotation->s = combine(7, "((", lit(this->left), ")", "!=", "(", lit(this->right), "))");
            this->annotation->type = 0;
            break;
        // case 12:
        //     this->annotation->s = combine(7, "((", lit(this->left), ")", "<>", "(", lit(this->right), "))");
        //     break;
    }
}

void evaluate_ast_if(struct if_ast* this)
{
	evaluate(this->cond);
    evaluate(this->then);
    evaluate(this->else_then);
    this->annotation->s = combine(7, "if(", lit(this->cond), "){", lit(this->then), "else{", lit(this->else_then), "}");
}

void evaluate_ast_while(struct while_ast* this)
{
    evaluate(this->cond);
    evaluate(this->then);
	this->annotation->s = combine(5, "while(", lit(this->cond), "){", lit(this->then), "}");
}

void evaluate_ast_procedure(struct procedure_ast* this)
{
    evaluate(this->to_run);
    this->annotation->s = combine(5, "void ", this->name, "(){", lit(this->to_run), "}");
}

void evaluate_ast_generic_list(struct generic_list_ast* this)
{
    evaluate(this->current);
    evaluate(this->next);

	this->annotation->s = append(this->current->annotation->s, this->next->annotation->s);
}

void destruct_generic_ast(struct generic_ast* to_destroy)
{
	// switch(to_destroy->type)
	// {
	// case 0:
	// 	destruct_ast_fork(to_destroy);
	// 	break;
	// case 1:
	// 	destruct_ast_if(to_destroy);
	// 	break;
	// case 2:
	// 	destruct_ast_while(to_destroy);
	// 	break;
	// case 3:
	// 	destruct_ast_procedure(to_destroy);
	// 	break;
	// case 4:
	// 	destruct_ast_generic_list(to_destroy);
	// 	break;
	// }
}

void destruct_ast_fork(struct fork_ast* to_destroy)
{
	destruct_generic_ast(to_destroy->left);
	destruct_generic_ast(to_destroy->right);
	free(to_destroy);
}

void destruct_ast_if(struct if_ast* to_destroy)
{
	destruct_generic_ast(to_destroy->cond);
	destruct_generic_ast(to_destroy->then);
	destruct_generic_ast(to_destroy->else_then);
	free(to_destroy);
}

void destruct_ast_while(struct while_ast* to_destroy)
{
	destruct_generic_ast(to_destroy->cond);
	destruct_generic_ast(to_destroy->then);
	free(to_destroy);
}

void destruct_ast_procedure(struct procedure_ast* to_destroy)
{
	destruct_generic_ast(to_destroy->to_run);
	free(to_destroy);
}

void destruct_ast_generic_list(struct generic_list_ast* to_destroy)
{
	destruct_generic_ast(to_destroy->next);
	destruct_generic_ast(to_destroy->current);
	free(to_destroy);
}

struct generic_ast* create_ast_fork(int op, struct generic_ast* left, struct generic_ast* right)
{
    // printf("HERE455\n");
    fflush(stdout);
	struct fork_ast* returning = abort_on_error_malloc(sizeof(struct fork_ast));
    returning->op = op;
	returning->left = left;
	returning->right = right;
	returning->type=0;
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_if(struct generic_ast* cond, struct generic_ast* then, struct generic_ast* else_then)
{
	struct if_ast* returning = abort_on_error_malloc(sizeof(struct if_ast));
	returning->cond = cond;
	returning->then = then;
	returning->else_then = else_then;
	returning->type=1;
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_while(struct generic_ast* cond, struct generic_ast* then)
{
	struct while_ast* returning = abort_on_error_malloc(sizeof(struct while_ast));
	returning->cond = cond;
	returning->then = then;
	returning->type=2;
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_procedure(struct generic_ast* to_run, char* name)
{
	struct procedure_ast* returning = abort_on_error_malloc(sizeof(struct procedure_ast));
    returning->name = name;
	returning->to_run = to_run;
	returning->type=3;
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_out(struct generic_ast* to_print, int nl)
{
	struct out_ast* returning = abort_on_error_malloc(sizeof(struct out_ast));
	returning->to_print = to_print;
	returning->type=7;
    returning->nl = nl;
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_procedure_call(char* name)
{
	struct procedure_call_ast* returning = abort_on_error_malloc(sizeof(struct procedure_call_ast));
	returning->name = name;
	returning->type=8;
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_generic_list(struct generic_ast* next, struct generic_ast* current)
{
    // printf("HERE509\n");
    fflush(stdout);
	struct generic_list_ast* returning = abort_on_error_malloc(sizeof(struct generic_list_ast));
	returning->next = next;
	returning->current = current;
	returning->type=4;
	return (struct generic_ast*) returning;
}
/* ### Function Syntax End ### */

/* EXTRA*/
void evaluate_ast_value(struct value_ast* this)
{
    if(this->data_type == -1){
        if(!ht_inside(types, this->data)){
            fprintf(stderr, "%s ∉ L (name %s not found)", this->data, this->data);
            error();
        }
        else{
            this->data_type = ht_get(types, this->data);
        }
    }
	this->annotation->type = this->data_type;
    this->annotation->s = string_from(this->data);
}

void evaluate_ast_decl(struct decl_ast* this)
{
    // printf("Here3");
	evaluate(this->becomes);

    // printf("EAD: %s\n", this->name);
    fflush(stdout);

    // printf("Here2");
    if(!ht_inside(types, this->name)){
        def_add((struct generic_ast*) this);
        switch(this->becomes->annotation->type){
            case 0:
                ht_set(types, this->name, 0);
                this->annotation->s = combine(5, "int ", this->name, "=", lit(this->becomes), ";");
                break;
            case 1:
                ht_set(types, this->name, 1);
                this->annotation->s = combine(5, "int ", this->name, "=", lit(this->becomes), ";");
                break;
            case 2:
                ht_set(types, this->name, 2);
                this->annotation->s = combine(5, "double ", this->name, "=", lit(this->becomes), ";");
                break;
        }
    }
    else{
        this->annotation->s = combine(4, this->name, "=", lit(this->becomes), ";");
    }


}

void destruct_ast_value(struct value_ast* to_destroy)
{
	
	free(to_destroy);
}

void destruct_ast_decl(struct decl_ast* to_destroy)
{
	destruct_generic_ast(to_destroy->becomes);
	free(to_destroy);
}

struct generic_ast* create_ast_value(int data_type, char* data)
{
    // printf("HERE572\n");
    fflush(stdout);
    if(data_type == -1){
        /* thus continues the great American tradition of can kicking*/
    }
	struct value_ast* returning = abort_on_error_malloc(sizeof(struct value_ast));
	returning->data_type = data_type;
	returning->data = data;
	returning->type=5;
    // printf("HERE583\n");
    fflush(stdout);
	return (struct generic_ast*) returning;
}

struct generic_ast* create_ast_decl(char* name, struct generic_ast* becomes)
{
    // printf("Here %s\n", name);
	struct decl_ast* returning = abort_on_error_malloc(sizeof(struct decl_ast));
	returning->name = name;
	returning->becomes = becomes;
	returning->type=6;
	return (struct generic_ast*) returning;
}

void setup(){
    types = ht_create( 65536 );
    true_head_proc = malloc(sizeof(struct ast_list));
    true_head_proc->next = (struct ast_list*) NULL;
    true_head_proc->current = (struct generic_ast*) NULL;
    current_head_proc = true_head_proc;

    true_head_def = malloc(sizeof(struct ast_list));
    true_head_def->next = (struct ast_list*) NULL;
    true_head_def->current = (struct generic_ast*) NULL;
    curr_head_def = true_head_def;
}

void print_out_headers(){
    // printf("HERE607");
    fflush(stdout);
    struct ast_list* current_nd = true_head_proc;
    // printf("HERE");
    fflush(stdout);
    while(current_nd->current){
        printf("void %s();", ((struct procedure_ast*) (current_nd->current))->name);
        current_nd = current_nd->next;
    }
}

void print_out_functions(){
    struct ast_list* current_nd = true_head_proc;
    while(current_nd->current){
        printf("%s", current_nd->current->annotation->s->str);
        current_nd->current->annotation->s = string_from("");
        current_nd = current_nd->next;
    }
}

void print_out_definitions(){
    struct ast_list* current_nd = true_head_def;
    while(current_nd->current){
        printf("%s", current_nd->current->annotation->s->str);
        current_nd->current->annotation->s = string_from("");
        current_nd = current_nd->next;
    }
}

void grand_print(struct generic_ast* head){
    // printf("HERE\n");
    fflush(stdout);
    printf("#include<stdio.h>\n");
    print_out_headers();
    print_out_definitions();
    print_out_functions();
    printf("int main(){");
    printf("%s", head->annotation->s->str);
    printf("}");
}

// int main(){
//     evaluate(create_ast_fork(1, create_ast_value(1, "17"), create_ast_value(1, "77")));
//     // printf("HERE1\n");
//     fflush(stdout);
//     struct generic_ast* evo = create_ast_decl("KettleChip", create_ast_fork(1, create_ast_value(1, "17"), create_ast_value(1, "77")));
//     evaluate(evo);
//     printf("\n\n%s", 
//         evo->annotation->s->str
//     );






//     int KettleChip=(((int)17)+((int)77));
// }