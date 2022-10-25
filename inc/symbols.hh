/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:18:28
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-24 19:04:29
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/symbols.hh
 */
#include <string>

using std::string;

class SymbolInfo
{
private:
    // todo
public:
    SymbolInfo();
    ~SymbolInfo();
};

class Symbol
{
private:
    string name;
    SymbolInfo *info;

public:
    Symbol();
    ~Symbol();
};


