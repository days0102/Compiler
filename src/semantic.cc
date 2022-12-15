/*
 * @Author: Outsider
 * @Date: 2022-11-21 20:13:24
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-15 13:43:05
 * @Description: 简要的语义分析
 * @FilePath: /compiler/src/semantic.cc
 */
#include "semantic.hh"

SymbolTable *stb;

void Prohead::semantic()
{
    auto res = stb->find(this->token->name);
    if (res == nullptr)
    {
        stb->add(this->token->name, new IdSymbol(this->token, this->token->name));
    }
    else
    {
        std::cout << "reuse head " << this->token << std::endl;
    }
}

void Expressions::semantic()
{
    auto it = this->explist.begin();
    for (; it != this->explist.end(); ++it)
        (*it)->semantic();
}

void Classbody::semantic()
{
    this->explist->semantic();
}

void Class::semantic()
{
    auto res = stb->find(this->token->name);
    if (res == nullptr)
    {
        stb->add(this->token->name, new IdSymbol(this->token, this->token->name));
        stb = stb->enter();
        this->classbody->semantic();
        stb = stb->exit();
    }
    else
    {
        std::cout << "redefine class " << this->token << std::endl;
        // std::cout << "first define at line " << cs->getline() << std::endl;
        // std::cout << "redeine define at line "  << std::endl;
    }
}

void Proclass::semantic()
{
    auto it = this->classes.begin();
    for (; it != this->classes.end(); ++it)
    {
        (*it)->semantic();
    }
}

void Evaluate::semantic()
{
    auto res = stb->globalFind(this->left->name);
    if (res == nullptr)
    {
        std::cout << "undefine id " << this->left->name << std::endl;
    }
    else
    {
        // todo 赋值号左右两边类型检查
    }
    this->right->semantic();
}

void Number::semantic() {}

void Object::semantic() {}

void Use::semantic()
{
    auto res = stb->find(this->exp->left->name);
    if (res == nullptr)
    {
        stb->add(this->exp->left->name, new IdSymbol(this->exp->left, this->exp->left->name));
    }
    else
    {
        std::cout << "redefine id " << this->exp->left->name << std::endl;
    }
    this->exp->right->semantic();
}

void Operation::semantic() {}

void Parameter::semantic() {}

void Parameters::semantic() {}

void Function::semantic() {}

void Call::semantic() {}

void initSymboltable()
{
    // 初始化添加关键字
}

void Program::semantic()
{
    stb = new SymbolTable();
    if (this->prohead != nullptr)
        this->prohead->semantic();
    if (this->proclass != nullptr)
        this->proclass->semantic();
}