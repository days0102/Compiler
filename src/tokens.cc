/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:16:24
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-02 09:07:10
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/tokens.cc
 */
#include "tokens.hh"

Token::Token(string str, int index) : name(str), index(index) {}
Token::Token(string str,TokenType type, int index) : name(str),type(type),index(index) {}
Token::~Token() {}

Tokentable::Tokentable() : idx(0) {}

Tokentable::~Tokentable() {}

/* set
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
*/

Token* Tokentable::add(string str,TokenType type){
    Token* tok=new Token(str,type,this->idx++);
    toks.push_back(tok);
    return tok;
}

Tokentable tokentable;