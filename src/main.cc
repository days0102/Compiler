/*
 * @Author: Outsider
 * @Date: 2022-10-05 20:08:20
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-16 14:45:13
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

bool crash = false;
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
    std::map<std::string, int> args;
    if (argc <= 1)
    {
        cout << "usage read filename" << endl;
        return 0;
    }
    for (int i = 2; i < argc; i++)
    {
        args[argv[i]]++;
    }

    if ((yyin = fopen(argv[1], "r")) == nullptr)
    {
        cout << "fopen" << argv[1] << "error" << endl;
        return 0;
    }

    /*
        -I 输出 LLVM IR 代码到文件   -i 控制台输出
        -l 控制台输出token序列
        -t 输出语法树到控制台
        -S 输出汇编代码到文件
        -B 输出符号表
        -o 生成目标代码
        -A 生成所有文件
        -a 输出所有内容
    */
    if (args.size() == 0)
    {
        args["-a"]++;
        args["-A"]++;
    }

    if (args.find("-a") != args.end() || args.find("-l") != args.end())
    {
        cout << "--------Tokens-----------" << endl;
        while (displaytoken(yylex()) != 0)
            ;
        cout << "-------------------------" << endl;
    }

    // initkeyword();
    // cout << "------------" << endl;
    // cout << "line: " << line << endl
    //      << "word: " << word << endl;

    yylineno = 1;
    fseek(yyin, 0, SEEK_SET);
    yyparse();
    cout << endl;
    if (crash)
    {
        cout << "Syntax error!" << endl;
        return 0;
    }

    if (ast_root == nullptr)
        return 1;

    if (args.find("-a") != args.end() || args.find("-t") != args.end())
    {
        cout << "--------Tree-----------" << endl;
        // ast_root->print(0);
        cout << to_string(ast_root->getNode()) << endl;
        cout << "-----------------------" << endl;
    }

    ast_root->semantic();
    if (crash)
    {
        cout << "Semantic error!" << endl;
        return 0;
    }

    // llvm::Value *v = ast_root->proclass->classes.front()->classbody->explist->explist.front()->CodeGen();
    llvm::Module *m = ast_root->CodeGen();
    if (crash)
    {
        cout << "Codegen error!" << endl;
        return 0;
    }

    if (args.find("-a") != args.end() || args.find("-i") != args.end())
    {
        cout << "--------LLVM IR-----------" << endl;
        m->print(llvm::outs(), nullptr, false, false);
        cout << "--------------------------" << endl;
    }
    if (args.find("-A") != args.end() || args.find("-I") != args.end())
        IRCode();
    if (args.find("-A") != args.end())
    {
        GenCode(0);
        GenCode(1);
    }
    else if (args.find("-o") != args.end() || args.size() == 0)
    {
        GenCode(1);
        if (args.find("-S") != args.end())
            GenCode(0);
    }
    else if (args.find("-S") != args.end())
        GenCode(0);

    cout << endl;
}