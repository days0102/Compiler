/*
 * @Author: Outsider
 * @Date: 2022-10-13 11:25:25
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-15 21:51:45
 * @Description: In User Settings Edit
 * @FilePath: /compiler/tree.h
 */

#include <list>

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
tnode::tnode(int line) : line(line) {}
tnode::~tnode() {}
int tnode::getline() { return line; }

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
Expression::Expression(int line) : tnode(line) {}
Expression::~Expression() {}

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
Prohead::Prohead(int line, Token *using_name) : Expression(line), using_name(using_name) {}
Prohead::Prohead(Token *using_name) : using_name(using_name) {}
Prohead::~Prohead() {}

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
Expressions::Expressions(int line, Expression *exp) : Expression(line)
{
    explist.push_back(exp);
}
Expressions *Expressions::add(Expression *exp)
{
    explist.push_back(exp);
    return this;
}
Expressions::~Expressions() {}

class Classbody : public Expression
{
private:
    Expressions *explist;

public:
    Classbody() = default;
    Classbody(int, Expressions *explist);
    ~Classbody();
};
Classbody::Classbody(int line, Expressions *explist) : Expression(line), explist(explist) {}
Classbody::~Classbody() {}

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
Class ::Class(int line, Token *token) : Expression(line), name(token) {}
Class ::Class(int line, Token *token, Classbody *body)
    : Expression(line), name(token), classbody(body) {}
Class *Class::add(Expression *exp)
{
    // body.push_back(exp);
    return this;
}
Class ::~Class() {}

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
Proclass::Proclass(int line) : Expression(line) {}
Proclass::Proclass(int line, Class *a) : Expression(line)
{
    classes.push_back(a);
}
Proclass *Proclass::add(Class *a)
{
    classes.push_back(a);
    return this;
}
Proclass::~Proclass() {}

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
Program::Program(int line, Prohead *head, Proclass *proclass)
    : tnode(line), prohead(head), proclass(proclass) {}
Program::~Program() {}

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
Evaluate::Evaluate(int line, Token *left, Expression *right)
    : Expression(line), left(left), right(right) {}
Evaluate::~Evaluate() {}

class Number : public Expression
{
private:
    Token *token;

public:
    Number() = default;
    Number(int line, Token *token);
    ~Number();
};
Number::Number(int line, Token *token) : token(token), Expression(line) {}
Number::~Number() {}

class Object : public Expression
{
private:
    Token *token;

public:
    Object() = default;
    Object(int line, Token *token);
    ~Object();
};
Object::Object(int line, Token *token) : token(token), Expression(line) {}
Object::~Object() {}

class Use : public Expression
{
private:
    Expression *exp;

public:
    Use(int line, Expression *exp);
    ~Use();
};
Use::Use(int line, Expression *exp) : Expression(line), exp(exp) {}
Use::~Use() {}

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
Operation::Operation(int line, Expression *left, char op, Expression *right)
    : Expression(line), left(left), op(op), right(right) {}
Operation::~Operation() {}

class Parameter : public Expression
{
private:
public:
    Parameter(/* args */);
    ~Parameter();
};
Parameter::Parameter(/* args */) {}
Parameter::~Parameter() {}

class Parameters : public Expression
{
private:
public:
    Parameters(/* args */);
    ~Parameters();
};
Parameters::Parameters(/* args */) {}
Parameters::~Parameters() {}

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
Function::Function() {}
Function::~Function() {}
