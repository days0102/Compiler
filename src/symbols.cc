/*
 * @Author: Outsider
 * @Date: 2022-10-24 09:42:02
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-16 16:36:02
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/symbols.cc
 */
#include <iostream>
#include "symbols.hh"

Symbol::Symbol(std::string name) : name(name) {}

Symbol::~Symbol() {}

IdSymbol::IdSymbol(Token *token, std::string name, std::string info) : token(token), Symbol(name), info(info) {}
void IdSymbol::show()
{
    std::cout << this->info << std::endl;
}

SymbolTable::SymbolTable()
{
    this->parent = nullptr;
}
SymbolTable::SymbolTable(SymbolTable *parent)
{
    this->parent = parent;
}
Symbol *SymbolTable::globalFind(std::string name)
{
    auto res = this->table.find(name);
    if (res != this->table.end())
    {
        return res->second;
    }
    if (this->parent != nullptr)
    {
        // 递归向父作用域查找
        return this->parent->find(name);
    }
    return nullptr;
}
Symbol *SymbolTable::find(std::string name)
{
    auto res = this->table.find(name);
    if (res != this->table.end())
    {
        return res->second;
    }
    return nullptr;
}

Symbol *SymbolTable::add(std::string name, Symbol *symbol)
{
    auto res = table.insert(make_pair(name, symbol));
    return symbol;
}

SymbolTable *SymbolTable::enter(std::string name) // 作用域名字
{
    auto stb = new SymbolTable(this);
    this->childen.insert(std::make_pair(name, stb));
    return stb;
}

SymbolTable *SymbolTable::exit()
{
    return this->parent;
}

void SymbolTable::show(int level)
{
    if (this->parent == nullptr)
        std::cout << "---global---" << std::endl;
    auto it = this->table.begin();
    for (; it != this->table.end(); ++it)
    {
        std::string str = std::string(level, ' ');
        std::cout << str;
        std::cout << it->first << "  ";
        it->second->show();
        auto child = this->childen.find(it->first);
        if (child != this->childen.end())
        {
            std::string str = std::string(level + 4, ' ');
            std::cout << str;
            std::cout << "---" << it->first << "---" << std::endl;
            child->second->show(level + 4);
        }
    }
}