/*
 * @Author: Outsider
 * @Date: 2022-10-11 20:22:15
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-31 09:28:03
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/tokens.hh
 */
#ifndef H_TOKENS
#define H_TOKENS
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
    string str;
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