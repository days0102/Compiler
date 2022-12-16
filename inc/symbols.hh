/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:18:28
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-16 16:36:41
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
    virtual void show() = 0;
    ~Symbol();
};

class IdSymbol : public Symbol
{
private:
    Token *token;
    std::string info;

public:
    IdSymbol(Token *, std::string, std::string);
    void show() override;
};

class SymbolTable
{
private:
    std::map<std::string, Symbol *> table;
    /*
    一个作用域，可能有多个子作用域
    一个作用域，自能有一个直接父作用域
    */
    SymbolTable *parent;                          // 外部作用域
    std::map<std::string, SymbolTable *> childen; // 内部作用域

public:
    SymbolTable();
    SymbolTable(SymbolTable *);
    Symbol *globalFind(std::string);
    Symbol *find(std::string); // 在当前作用域查找
    Symbol *add(std::string, Symbol *);
    SymbolTable *enter(std::string); // 进入一个作用域
    SymbolTable *exit();             // 退出作用域

    void show(int level); // 输出符号表
};

#endif