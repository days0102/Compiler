/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:53:44
 * @LastEditors: Outsider
 * @LastEditTime: 2022-10-23 21:19:43
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/tree.cc
 */
#include "tree.hh"

tnode::tnode(int line) : line(line) {}
tnode::~tnode() {}
int tnode::getline() { return line; }

Expression::Expression(int line) : tnode(line) {}
Expression::~Expression() {}

Prohead::Prohead(int line, Token *using_name) : Expression(line), using_name(using_name) {}
Prohead::Prohead(Token *using_name) : using_name(using_name) {}
Prohead::~Prohead() {}


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


Classbody::Classbody(int line, Expressions *explist) : Expression(line), explist(explist) {}
Classbody::~Classbody() {}

Class ::Class(int line, Token *token) : Expression(line), name(token) {}
Class ::Class(int line, Token *token, Classbody *body)
    : Expression(line), name(token), classbody(body) {}
Class *Class::add(Expression *exp)
{
    // body.push_back(exp);
    return this;
}
Class ::~Class() {}

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


Program::Program(int line, Prohead *head, Proclass *proclass)
    : tnode(line), prohead(head), proclass(proclass) {}
Program::~Program() {}

Evaluate::Evaluate(int line, Token *left, Expression *right)
    : Expression(line), left(left), right(right) {}
Evaluate::~Evaluate() {}

Number::Number(int line, Token *token) : token(token), Expression(line) {}
Number::~Number() {}

Object::Object(int line, Token *token) : token(token), Expression(line) {}
Object::~Object() {}

Use::Use(int line, Expression *exp) : Expression(line), exp(exp) {}
Use::~Use() {}


Operation::Operation(int line, Expression *left, char op, Expression *right)
    : Expression(line), left(left), op(op), right(right) {}
Operation::~Operation() {}

Parameter::Parameter() {}
Parameter::~Parameter() {}


Parameters::Parameters() {}
Parameters::~Parameters() {}


Function::Function() {}
Function::~Function() {}