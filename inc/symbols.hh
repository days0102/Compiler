/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:18:28
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-22 10:54:12
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/symbols.hh
 */
#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <string>
#include <map>
#include "tokens.hh"

using std::string;

class Symbol
{
private:
    string name;

public:
    Symbol(std::string);
    ~Symbol();
};

class IdSymbol : public Symbol
{
private:
    Token *token;

public:
    IdSymbol(Token *, std::string);
};

class SymbolTable
{
private:
    std::map<std::string, Symbol *> table;
    /*
    一个作用域，可能有多个子作用域
    一个作用域，自能有一个直接父作用域
    */
    SymbolTable *parent;              // 外部作用域
    std::list<SymbolTable *> childen; // 内部作用域

public:
    SymbolTable();
    SymbolTable(SymbolTable *);
    Symbol *find(std::string);
    Symbol *add(std::string, Symbol *);
    SymbolTable *enter();
    SymbolTable *exit();
};

#endif