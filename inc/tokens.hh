/*
 * @Author: Outsider
 * @Date: 2022-10-11 20:22:15
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-23 20:52:19
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/tokens.h
 */
#ifndef TOKENS 
#define TOKENS
#include<string>
using std::string;

class Token
{
private:
    string str;
    int index;

public:
    Token() = default;
    Token(string str, int index);
    ~Token();
};

#endif