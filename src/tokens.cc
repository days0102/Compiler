/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:16:24
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-25 15:08:54
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/tokens.cc
 */
#include "tokens.hh"

Token::Token(string str, int index) : str(str), index(index) {}
Token::~Token() {}

Tokentable::Tokentable() : idx(0) {}

Tokentable::~Tokentable() {}


Token *Tokentable::add(string name)
{
    Token *tok = new Token(name);
    auto res = this->toks.insert(tok);
    if (res.second)
    {
        tok->index = idx++;
        return tok;
    }
    else
    {
        delete tok;
        return *res.first;
    }
}

Tokentable tokentable;