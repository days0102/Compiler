/*
 * @Author: Outsider
 * @Date: 2022-10-24 09:42:02
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-22 10:54:28
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/symbols.cc
 */

#include "symbols.hh"

Symbol::Symbol(std::string name) : name(name) {}

Symbol::~Symbol() {}

IdSymbol::IdSymbol(Token *token, std::string name) : token(token), Symbol(name) {}

SymbolTable::SymbolTable()
{
    this->parent = nullptr;
}
SymbolTable::SymbolTable(SymbolTable *parent)
{
    this->parent = parent;
}
Symbol *SymbolTable::find(std::string name)
{
    auto res = this->table.find(name);
    if (res != this->table.end())
    {
        return res->second;
    }
    if (this->parent != nullptr)
    {
        return this->parent->find(name);
    }
    return nullptr;
}

Symbol *SymbolTable::add(std::string name, Symbol *symbol)
{
    auto res = table.insert(make_pair(name, symbol));
    return symbol;
}

SymbolTable *SymbolTable::enter()
{
    auto stb = new SymbolTable(this);
    this->childen.push_back(stb);
    return stb;
}

SymbolTable *SymbolTable::exit()
{
    return this->parent;
}