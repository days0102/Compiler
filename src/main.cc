/*
 * @Author: Outsider
 * @Date: 2022-10-05 20:08:20
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-31 09:49:07
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/main.cc
 */
#include <iostream>
#include "tokens.hh"
// #include "bison.tab.hh"

using std::cout;
using std::endl;
using std::ostream;

extern FILE *yyin;
extern int yylex();
extern int yyparse();

extern Tokentable tokentable;
extern int displaytoken(int);

int line = 1, word = 0;
/*
void initkeyword()
{
    tokentable.add("using");
    tokentable.add("class");
    tokentable.add("str");
    tokentable.add("int");
    tokentable.add("use");
    tokentable.add("ftn");
}
*/

int main(int argc, char **argv)
{
    if (argc <= 1)
    {
        cout << "usage read filename" << endl;
        return 0;
    }
    if ((yyin = fopen(argv[1], "r")) < 0)
    {
        cout << "fopen" << argv[1] << "error" << endl;
        return 0;
    }
    while (displaytoken(yylex()) != 0)
        ;
    // initkeyword();
    
    cout<<"-------------------"<<endl;
    fseek(yyin,0,SEEK_SET);
    yyparse();
    cout << endl;
    cout << "line: " << line << endl
         << "word: " << word << endl;
}