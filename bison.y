%{
#include "lex.yy.c"
#include "bison.tab.h"
#include "tree.h"

extern int line;
void yyerror(char *s);


Program* ast_root;
%}

%union{
    int number;
    char* objname;
    char* keyword;
    class Program* type_program;
    class Expression* type_expression;
    class Expressions* type_expressions;
    class Token* type_token;
    class Prohead* type_prohead;
    class Proclass* type_proclass;
    class Class* type_class;
    class Classbody* type_classbody;
    class Evaluate* type_evaluate;
    
}

/*language "c++"*/ /* c++支持 */

%token USE 1 NUM 2 STRING 3 USING 4 CLASS 5 OBJECT 6

%type<type_token> NUM STRING OBJECT

%type<type_program>t_program
%type<type_classbody> t_classbody
%type<type_proclass> t_classlist
%type<type_class> t_class
%type<type_prohead> t_using
%type<type_expressions> t_explist
%type<type_expression> t_exp 
%type<type_evaluate>t_evaluation

/* %nonassoc '=' */

%start t_program
%%

t_program
    : t_using
    { ast_root=new Program(yylineno,$1,nullptr); } 
    | t_classlist
    { ast_root=new Program(yylineno,nullptr,$1);} 
    | t_using t_classlist
    {   
        ast_root=new Program(yylineno,$1,$2);
        printf("ast_root:%p line %d",ast_root,ast_root->getline());
    };

t_using
    : USING OBJECT
    { $$=new Prohead(yylineno,$2);}
    ;
t_classlist
    : t_class
    { $$=new Proclass(yylineno,$1);}
    | t_classlist t_class
    { $$=$1->add($2);}
    ;

t_class
    : CLASS OBJECT '{' t_classbody '}' 
    { $$=new Class(yylineno,$2,$4);}
    ;
t_classbody:    {$$=nullptr;}
    | t_explist
    { $$=new Classbody(yylineno,$1);}
    ;

t_explist:
    | t_exp
    { $$=new Expressions(yylineno,$1);} 
    | t_explist t_exp
    { $$=$1->add($2);}
    ;

t_exp :
    | NUM
    { $$=new Number(yylineno,$1);}
    | STRING
    | OBJECT
    { $$=new Object(yylineno,$1);}
    | USE t_evaluation 
    { $$=new Use(yylineno,$2);}
    | t_exp '+' t_exp
    { $$=new Operation(yylineno,$1,'+',$3);}
    | t_exp '-' t_exp
    { $$=new Operation(yylineno,$1,'-',$3);}
    | t_exp '*' t_exp
    { $$=new Operation(yylineno,$1,'*',$3);}
    | t_exp '/' t_exp
    { $$=new Operation(yylineno,$1,'/',$3);}
    ;

t_evaluation
    : OBJECT '=' t_exp
    { $$=new Evaluate(yylineno,$1,$3);}
    | OBJECT '=' t_exp ',' t_evaluation
    
    ;
%%

void yyerror(char *msg)
{
    printf("yyerror: line %d %s at '%s'\n",yylineno,msg,yytext);
    /* yyparse(); */
}