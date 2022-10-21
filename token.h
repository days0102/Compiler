/*
 * @Author: Outsider
 * @Date: 2022-10-11 20:22:15
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-15 20:59:15
 * @Description: In User Settings Edit
 * @FilePath: /compiler/token.h
 */

using std::string;
int index=0;
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

Token::Token(string str, int index) : str(str), index(index) {}
Token::~Token() {}
