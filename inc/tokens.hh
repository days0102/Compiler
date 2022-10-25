/*
 * @Author: Outsider
 * @Date: 2022-10-11 20:22:15
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-25 15:34:04
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/tokens.hh
 */
#ifndef H_TOKENS
#define H_TOKENS
#include <string>
#include <set>

using std::string;

class Token
{
private:
public:
    string str;
    int index;

    Token() = default;
    Token(string str, int index = 0);
    ~Token();
};

class Tokentable
{
private:
    class Compare
    {
    public:
        bool operator()(Token *tl, Token *tr) const
        {
            return tl->str < tr->str;
        }
    };
    std::set<Token *, Compare> toks;
    int idx;

public:
    Tokentable();
    ~Tokentable();
    Token *add(string);
};

#endif