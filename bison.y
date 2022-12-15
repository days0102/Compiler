%{
#include "lex.yy.c"
#include "bison.tab.hh"
#include "tree.hh"

extern int line;
void yyerror(char *s);


Program* ast_root;
%}

/*声明要使用的类型*/
%union{
    class Program* type_program;    /*整个程序*/
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
/* 定义关键字对应的token值，相当于宏定义 */
%token USE 1000 NUM 1001 USING 1002 CLASS 1003 OBJECT 1004 FTN 1005
/* 声明token值对应对象的类型 */
%type<type_token> NUM STRING OBJECT

%type<type_program>t_program
%type<type_classbody> t_classbody
%type<type_proclass> t_classlist
%type<type_class> t_class
%type<type_prohead> t_using
%type<type_expressions> t_explist
%type<type_expression> t_exp
%type<type_evaluate>t_evaluation

%nonassoc '='
%left '+' '-'
%left '*' '/'

%start t_program /*起始点*/

%%
/* 翻译规则 */
/*
    A:  a1 { 语义动作1 }
    | a2 { 语义动作2 }
    ...
    ;
    $$ 表示与产生式左部非终结符号相关的位置
    $i 表示与产生式右部第i个文法符号相关的属性值
*/

/*程序由包含的头和多个class组成*/
t_program 
    : t_using
    { ast_root=new Program(yylineno,$1,nullptr); } 
    | t_classlist
    { ast_root=new Program(yylineno,nullptr,$1);} 
    | t_using t_classlist
    {   
        ast_root=new Program(yylineno,$1,$2);
        /*cout<<"ast_root: "<<ast_root<<"\tline: "<<ast_root->getline()<<endl;*/
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
t_classbody
    :   /*空规则，允许类内为空*/
    {$$=nullptr;}
    | t_explist
    { $$=new Classbody(yylineno,$1);}
    ;

t_explist:
    | t_exp
    { $$=new Expressions(yylineno,$1);} 
    | t_explist t_exp
    { $$=$1->add($2);}
    ;

/* 表达式 */
t_exp :
    | NUM
    { $$=new Number(yylineno,$1);}
    | OBJECT
    { $$=new Object(yylineno,$1);}
    | USE t_evaluation
    { $$=new Use(yylineno,$2);}
    | t_evaluation  /*赋值表达式*/
    { $$=$1;}
    | t_exp '+' t_exp
    { $$=new Operation(yylineno,$1,'+',$3);}
    | t_exp '-' t_exp
    { $$=new Operation(yylineno,$1,'-',$3);}
    | t_exp '*' t_exp
    { $$=new Operation(yylineno,$1,'*',$3);}
    | t_exp '/' t_exp
    { $$=new Operation(yylineno,$1,'/',$3);}
    |'(' t_exp ')'
    { $$=$2;}
    | OBJECT '(' t_exp ')'      /*函数调用*/
    { $$=new Call(yylineno,$1,$3);}
    | OBJECT '(' ')'
    { $$=new Call(yylineno,$1);}
    ;

/* 赋值 */
t_evaluation
    : OBJECT '=' t_exp
    { $$=new Evaluate(yylineno,$1,$3);}
    ;
%%
/*实现yyerror函数，在遇到错误时会执行改函数*/
void yyerror(char *msg)
{
    cout<<"yyerror: "<<msg<<" at line "<<yylineno<<" with \""<<yytext<<"\""<<endl;
    /* yyparse(); */
}