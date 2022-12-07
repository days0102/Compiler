/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:53:44
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-05 23:14:45
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/tree.cc
 */
#include "tree.hh"
#include "codegen.hh"

tnode::tnode(int line) : line(line) {}
tnode::~tnode() {}
int tnode::getline() { return line; }

Expression::Expression(int line) : tnode(line) {}
void Expression::print(int level) {}
Expression::~Expression() {}

Prohead::Prohead(int line, Token *using_name) : Expression(line), token(using_name) {}
Prohead::Prohead(Token *using_name) : token(using_name) {}
void Prohead::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "USING " << this->token->name << endl;
}
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
void Expressions::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    auto it = this->explist.begin();
    for (; it != this->explist.end(); ++it)
    {
        (*it)->print(level + 2);
    }
}
Expressions::~Expressions() {}

Classbody::Classbody(int line, Expressions *explist) : Expression(line), explist(explist) {}

void Classbody::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    this->explist->print(level);
}
Classbody::~Classbody() {}

Class ::Class(int line, Token *token) : Expression(line), token(token) {}
Class ::Class(int line, Token *token, Classbody *body)
    : Expression(line), token(token), classbody(body) {}

void Class::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "CLASS " << this->token->name << endl;
    if (this->classbody != nullptr)
        this->classbody->print(level);
}
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

void Proclass::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    auto it = this->classes.begin();
    for (; it != this->classes.end(); ++it)
    {
        (*it)->print(level);
    }
}
Proclass::~Proclass() {}

Program::Program(int line, Prohead *head, Proclass *proclass)
    : tnode(line), prohead(head), proclass(proclass) {}
void Program::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    this->prohead->print(level);
    this->proclass->print(level);
}
Program::~Program() {}

Evaluate::Evaluate(int line, Token *left, Expression *right)
    : Expression(line), left(left), right(right) {}

void Evaluate::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "EVALUATE " << this->left->name << endl;
    this->right->print(level);
}
Evaluate::~Evaluate() {}

Number::Number(int line, Token *token) : token(token), Expression(line)
{
    this->val = stold(this->token->name);
}

void Number::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "NUMBER " << this->token->name << endl;
}
Number::~Number() {}

Object::Object(int line, Token *token) : token(token), Expression(line) {}

void Object::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "OBJECT " << this->token->name << endl;
}
Object::~Object() {}

// todo
Use::Use(int line, Evaluate *exp) : Expression(line), exp(exp) {}

void Use::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "USE " << endl;
    this->exp->print(level + 2);
}
Use::~Use() {}

Operation::Operation(int line, Expression *left, char op, Expression *right)
    : Expression(line), left(left), op(op), right(right) {}

void Operation::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
    cout << "OPERATION " << this->op << endl;
    this->left->print(level + 2);
    this->right->print(level + 2);
}
Operation::~Operation() {}

Parameter::Parameter() {}

void Parameter::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
}
Parameter::~Parameter() {}

Parameters::Parameters() {}

void Parameters::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
}
Parameters::~Parameters() {}

Function::Function() {}

void Function::print(int level)
{
    std::string str = std::string(level, ' ');
    cout << str;
}
Function::~Function() {}

Call::Call(int line, Token *name) : token(name), Expression(line) {}
Call::Call(int line, Token *name, Expression *arg) : token(name), Expression(line)
{
    this->args.push_back(arg);
}
void Call::print(int level)
{
}