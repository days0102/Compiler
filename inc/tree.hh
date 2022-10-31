/*
 * @Author: Outsider
 * @Date: 2022-10-13 11:25:25
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-26 16:17:00
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/tree.hh
 */

#include <list>
#include "tokens.hh"

/** abstract syntax tree node */
class tnode // tree noe
{
private:
    int line;

public:
    tnode() = default;
    tnode(int line);
    virtual ~tnode();

    int getline();
};

/**
 * @description:
 * @return {*}
 */
class Expression : public tnode
{
private:
public:
    Expression() = default;
    Expression(int line);
    ~Expression();
};

class Prohead : public Expression
{
private:
    Token *using_name;

public:
    Prohead() = default;
    Prohead(int line, Token *using_name);
    Prohead(Token *using_name);
    ~Prohead();
};

class Expressions : public Expression
{
private:
    std::list<Expression *> explist;

public:
    Expressions() = default;
    Expressions(int line, Expression *exp);
    Expressions *add(Expression *);
    ~Expressions();
};

class Classbody : public Expression
{
private:
    Expressions *explist;

public:
    Classbody() = default;
    Classbody(int, Expressions *explist);
    ~Classbody();
};

class Class : public Expression
{
private:
    Token *name;
    Classbody *classbody;

public:
    Class() = default;
    Class(int, Token *);
    Class(int, Token *, Classbody *);
    Class *add(Expression *);
    ~Class();
};

class Proclass : public Expression
{
private:
    std::list<Class *> classes;

public:
    Proclass() = default;
    Proclass(int);
    Proclass(int, Class *);
    Proclass *add(Class *);
    ~Proclass();
};

class Program : public tnode
{
private:
    Prohead *prohead;
    Proclass *proclass;

public:
    Program() = default;
    Program(int line, Prohead *head, Proclass *proclass);

    ~Program();
};

class Evaluate : public Expression
{
private:
    Token *left;
    Expression *right;

public:
    Evaluate() = default;
    Evaluate(int, Token *, Expression *);
    ~Evaluate();
};

class Number : public Expression
{
public:
    Token *token;
    
    Number() = default;
    Number(int line, Token *token);
    ~Number();
};

class Object : public Expression
{
private:
    Token *token;

public:
    Object() = default;
    Object(int line, Token *token);
    ~Object();
};

class Use : public Expression
{
private:
    Expression *exp;

public:
    Use(int line, Expression *exp);
    ~Use();
};

class Operation : public Expression
{
private:
    Expression *left;
    char op;
    Expression *right;

public:
    Operation() = default;
    Operation(int, Expression *, char, Expression *);
    ~Operation();
};

class Parameter : public Expression
{
private:
    string name;
    Token* type; // or not

public:
    Parameter();
    ~Parameter();
};

class Parameters : public Expression
{
private:
    

public:
    Parameters();
    ~Parameters();
};

class Function : public Expression
{
private:
    Token *name;
    Token *returntype;
    Parameters *parameters;
    Expressions *explist;

public:
    Function();
    ~Function();
};
