%{

#include<iostream>
#include "tokens.hh"
#include "bison.tab.hh"

using std::cout;
using std::endl;

extern int line;
extern int word;
extern Tokentable tokentable;
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
{USING}     {return (USING);}
{CLASS}     {return (CLASS);}
{USE}       {return (USE);}
{FTN}       {return (FTN);}

    /* {DIGIT}     {printf("digit: %s\n",yytext); return INT;}
    {LETTER}    {printf("letter: %s\n",yytext); word++; return STRING; } */
{OBJECT}    {yylval.type_token=tokentable.add(yytext,Id);return (OBJECT); }

{DIGIT}     {yylval.type_token=tokentable.add(yytext,Num); return NUM;}
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
      cout<<" Num\t->  "<<yylval.type_token->str<<endl;
      break;
    case 1005:
      cout<<" Id\t->  "<<yylval.type_token->str<<endl;
      break;
  }
  return token;
}