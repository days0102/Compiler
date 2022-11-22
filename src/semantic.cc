/*
 * @Author: Outsider
 * @Date: 2022-11-21 20:13:24
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-22 11:13:42
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/semantic.cc
 */
#include "semantic.hh"

SymbolTable *stb;

void check_using(class Prohead *head)
{
    auto res = stb->find(head->name->str);
    if (res == nullptr)
    {
        stb->add(head->name->str, new IdSymbol(head->name, head->name->str));
    }
    else
    {
        std::cout << "reuse head " << head->name << std::endl;
    }
}

void check_expressions(Expressions *exps)
{
    auto it = exps->explist.begin();
    for (; it != exps->explist.end(); ++it)
    {
    }
}

void check_classbody(Classbody *cb)
{
    check_expressions(cb->explist);
}

void check_class(Class *cs)
{
    auto res = stb->find(cs->name->str);
    if (res == nullptr)
    {
        stb->add(cs->name->str, new IdSymbol(cs->name, cs->name->str));
        stb = stb->enter();
        check_classbody(cs->classbody);
        stb = stb->exit();
    }
    else
    {
        std::cout << "redefine class " << cs->name << std::endl;
        // std::cout << "first define at line " << cs->getline() << std::endl;
        // std::cout << "redeine define at line "  << std::endl;
    }
}

void check_classess(class Proclass *classes)
{
    auto it = classes->classes.begin();
    for (; it != classes->classes.end(); ++it)
    {
        check_class(*it);
    }
}

void semantics(Program *ast_root)
{
    stb = new SymbolTable();
    check_using(ast_root->prohead);
    check_classess(ast_root->proclass);
}