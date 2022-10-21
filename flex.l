%{

#include<iostream>
#include "token.h"
#include "bison.tab.h"

using std::cout;
using std::endl;

extern int line;
extern int word;
extern int index;
%}

%option yylineno

DIGIT   [0-9]+

OBJECT [_a-z][a-zA-Z0-9]*

USING   using
CLASS   class
USE     use
FTN     ftn

%%
  
  /* keyword map */
{USING}     {yylval.keyword=yytext;return (USING);}
{CLASS}     {yylval.keyword=yytext;return (CLASS);}
{USE}       {yylval.keyword=yytext;return (USE);}
{FTN}       {yylval.keyword=yytext;return (FTN);}

    /* {DIGIT}     {printf("digit: %s\n",yytext); return INT;}
    {LETTER}    {printf("letter: %s\n",yytext); word++; return STRING; } */
{OBJECT}    {yylval.type_token=new Token(yytext,index++);return (OBJECT); }

{DIGIT}     {yylval.type_token=new Token(yytext,index++);; return NUM;}
 /* {LETTER}    {printf("letter: %s\n",yytext);yylval.objname=yytext; return STRING;} */
"+"         {return int('+');}
"-"         {return int('-');}
"*"         {return int('*');}
"/"         {return int('/');}
"="         {return int('=');}
","         {return int(',');}
"{"         {return int('{');}
"}"         {return int('}');}
"\n"        {line++;}
[ \t]       {}
.           {}
%%

