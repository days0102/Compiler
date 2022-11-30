/*
 * @Author: Outsider
 * @Date: 2022-10-05 20:08:20
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-29 17:17:45
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/main.cc
 */
#include <iostream>
#include "tree.hh"
#include "tokens.hh"
#include "semantic.hh"
#include "codegen.hh"
// #include "bison.tab.hh"
// #include "llvm/Support/raw_ostream.h"
// #include "llvm/IR/Module.h"

using std::cout;
using std::endl;
using std::ostream;

extern FILE *yyin;
extern int yylex();
extern int yyparse();
extern int yylineno;

extern Program *ast_root;

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
    if ((yyin = fopen(argv[1], "r")) == nullptr)
    {
        cout << "fopen" << argv[1] << "error" << endl;
        return 0;
    }
    while (displaytoken(yylex()) != 0)
        ;
    // initkeyword();
    cout << "------------" << endl;
    cout << "line: " << line << endl
         << "word: " << word << endl;

    cout << "-------------------" << endl;
    yylineno = 1;
    fseek(yyin, 0, SEEK_SET);
    yyparse();
    cout << endl;

    // ast_root->print(0);
    cout << to_string(ast_root->getNode()) << endl;

    ast_root->semantic();

    // llvm::Value *v = ast_root->proclass->classes.front()->classbody->explist->explist.front()->CodeGen();
    llvm::Module *m = ast_root->CodeGen();
    m->print(llvm::outs(), nullptr,false,false);

    ObjectCode();

    cout << endl;
}