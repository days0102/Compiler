/*
 * @Author: Outsider
 * @Date: 2022-10-11 20:22:15
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-23 09:17:26
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/tokens.hh
 */
#ifndef TOKENS_H
#define TOKENS_H
#include <string>
#include <list>

using std::string;

enum TokenType
{
    Id,
    Num,
    Keyword
};

class Token
{
private:
public:
    string name;
    int index;
    TokenType type;

    Token() = default;
    Token(string str, int index = 0);
    Token(string str,TokenType type, int index=0);
    ~Token();
};

class Tokentable
{
private:
    /* set
    class Compare
    {
    public:
        bool operator()(Token *tl, Token *tr) const
        {
            return tl->str < tr->str;
        }
    };
    std::set<Token *, Compare> toks;
    */

    std::list<Token*>toks;
    int idx;

public:
    Tokentable();
    ~Tokentable();
    Token *add(string,TokenType);
};

#endif