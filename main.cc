/*
 * @Author: Outsider
 * @Date: 2022-10-05 20:08:20
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-12 11:14:44
 * @Description: In User Settings Edit
 * @FilePath: /compiler/main.cc
 */
#include <iostream>

using std::cout;
using std::endl;
using std::ostream;

extern FILE *yyin;
extern int yylex();
extern int yyparse();

int line = 1, word = 0;

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
    // while (yylex() != 0)
    //     ;

    yyparse();
    cout << endl;
    cout << "line: " << line << endl
         << "word: " << word << endl;
}