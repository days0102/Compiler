/*
 * @Author: Outsider
 * @Date: 2022-10-23 20:53:44
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-01 20:21:55
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/tree.cc
 */
#include<iostream>
#include "tree.hh"
#include "codegen.hh"

using std::cout;
using std::endl;

tnode::tnode(int line) : line(line) {}
tnode::~tnode() {}
int tnode::getline() { return line; }

Expression::Expression(int line) : tnode(line) {}
Expression::~Expression() {}

Prohead::Prohead(int line, Token *using_name) : Expression(line), name(using_name) {}
Prohead::Prohead(Token *using_name) : name(using_name) {}
llvm::Value* Prohead::CodeGen() {
    return nullptr;
}
void Prohead::print(){
    cout<<"using "<<this->name->str<<endl;
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
llvm::Value* Expressions::CodeGen() {
    return nullptr;
}
Expressions::~Expressions() {}


Classbody::Classbody(int line, Expressions *explist) : Expression(line), explist(explist) {}
llvm::Value* Classbody::CodeGen() {
    return nullptr;
}
Classbody::~Classbody() {}

Class ::Class(int line, Token *token) : Expression(line), name(token) {}
Class ::Class(int line, Token *token, Classbody *body)
    : Expression(line), name(token), classbody(body) {}
llvm::Value* Class::CodeGen(){
    return nullptr;
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
llvm::Value* Proclass::CodeGen(){
    return nullptr;
}
Proclass::~Proclass() {}


Program::Program(int line, Prohead *head, Proclass *proclass)
    : tnode(line), prohead(head), proclass(proclass) {}
void Program::print(){
    this->prohead->print();
}
Program::~Program() {}

Evaluate::Evaluate(int line, Token *left, Expression *right)
    : Expression(line), left(left), right(right) {}
llvm::Value* Evaluate::CodeGen(){
    return nullptr;
}
Evaluate::~Evaluate() {}

Number::Number(int line, Token *token) : token(token), Expression(line) {
    this->val=stold(this->token->str);
}
extern llvm::LLVMContext TheContext;
llvm::Value* Number::CodeGen(){
    return llvm::ConstantInt::get(llvm::Type::getInt64Ty(TheContext), val, true);
}
Number::~Number() {}

Object::Object(int line, Token *token) : token(token), Expression(line) {}
llvm::Value* Object::CodeGen(){
    return nullptr;
}
Object::~Object() {}

Use::Use(int line, Expression *exp) : Expression(line), exp(exp) {}
llvm::Value* Use::CodeGen(){
    return nullptr;
}
Use::~Use() {}


Operation::Operation(int line, Expression *left, char op, Expression *right)
    : Expression(line), left(left), op(op), right(right) {}
llvm::Value* Operation::CodeGen(){
    return nullptr;
}
Operation::~Operation() {}

Parameter::Parameter() {}
llvm::Value* Parameter::CodeGen(){
    return nullptr;
}
Parameter::~Parameter() {}


Parameters::Parameters() {}
llvm::Value* Parameters::CodeGen(){
    return nullptr;
}
Parameters::~Parameters() {}


Function::Function() {}
llvm::Value* Function::CodeGen(){
    return nullptr;
}
Function::~Function() {}