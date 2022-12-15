%{
/* flex 常用变量和函数
  yyin  FILE*类型，指向Lex输入文件(缺省时指向标准输入)
  yyout FILE*类型，指向Lex输出文件(缺省时指向标准输出)
  yytext  char*类型，指向与识别规则中的一个正规式匹配的单词的首字符
  yyleng  int类型，记录与识别规则中正规式匹配的单词长度
  yylex() 从该函数开始分析由lex自动生成
  yywrap()  文件结束处理函数，如果返回1则停止解析。可以用来解析多个文件
*/
#include<iostream>
#include "tokens.hh"
#include "bison.tab.hh"

using std::cout;
using std::endl;

extern int line;
extern int word;
extern Tokentable tokentable;
%}
/* flex内置的行号跟踪变量 */
%option yylineno
/*整数*/
DIGIT   [0-9]+
/*变量名*/
OBJECT [_a-z][a-zA-Z0-9]*
/*关键字*/
USING   using
CLASS   class
USE     use
FTN     ftn

%%
  
  /* 关键字 相应的值定义在bison.y %token */
{USING}     {return (USING);}
{CLASS}     {return (CLASS);}
{USE}       {return (USE);}
{FTN}       {return (FTN);}

    /* {DIGIT}     {printf("digit: %s\n",yytext); return INT;}
    {LETTER}    {printf("letter: %s\n",yytext); word++; return STRING; } */
    /* yytext保存了识别到的字符串，通过识别到的字符串生成Token类，
      将生成的Token保存到token列表中（仅调试和输出时使用），并将其绑定到yylval中供bison.y使用*/
{OBJECT}    {yylval.type_token=tokentable.add(yytext,Id);return (OBJECT); }

{DIGIT}     {yylval.type_token=tokentable.add(yytext,Num); return NUM;}
 /* {LETTER}    {printf("letter: %s\n",yytext);yylval.objname=yytext; return STRING;} */
 /* 直接返回字符，以ASCII码作为token值 */
"+"         {return int('+');}
"-"         {return int('-');}
"*"         {return int('*');}
"/"         {return int('/');}
"="         {return int('=');}
","         {return int(',');}
"{"         {return int('{');}
"}"         {return int('}');}
"("         {return int('(');}
")"         {return int(')');}
"\n"        {line++;}
 /* 忽略空格 */
[ \t]       {}
 /* 忽略未匹配到的字符 */
.           {}
 /*注释*/
@@(.)*      {}
%%

  /*输出lexer分析的Token*/
int displaytoken(int token){
  switch(token){
    case (USING):
      cout<<" Key\t->  "<<"using"<<endl;
      break;
    case (CLASS):
      cout<<" Key\t->  "<<"class"<<endl;
      break;    
    case (USE):
      cout<<" Key\t->  "<<"use"<<endl;
      break; 
    case (FTN):
      cout<<" Key\t->  "<<"ftn"<<endl;
      break; 
    case ('+'):
      cout<<" Sym\t->  "<<'+'<<endl;
      break;
    case ('-'):
      cout<<" Sym\t->  "<<'-'<<endl;
      break;
    case ('*'):
      cout<<" Sym\t->  "<<'*'<<endl;
      break;
    case ('/'):
      cout<<" Sym\t->  "<<'/'<<endl;
      break;
    case ('='):
      cout<<" Sym\t->  "<<'='<<endl;
      break;
    case (','):
      cout<<" Sym\t->  "<<','<<endl;
      break;
    case ('{'):
      cout<<" Sym\t->  "<<'{'<<endl;
      break;
    case ('}'):
      cout<<" Sym\t->  "<<'}'<<endl;
      break;
    case 1001:
      cout<<" Num\t->  "<<yylval.type_token->name<<endl;
      break;
    case 1004:
      cout<<" Id\t->  "<<yylval.type_token->name<<endl;
      break;
  }
  return token;
}