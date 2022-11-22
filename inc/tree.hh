/*
 * @Author: Outsider
 * @Date: 2022-10-13 11:25:25
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-21 20:47:39
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/tree.hh
 */
#ifndef TREE_H
#define TREE_H

#include <list>
#include "tokens.hh"
#include "print.hh"

#include "llvm/IR/Value.h"

/** abstract syntax tree node */
class tnode // tree noe
{
private:
    int line;

public:
    tnode() = default;
    tnode(int line);
    virtual ~tnode();
    virtual void print(int level) = 0;
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
    virtual llvm::Value *CodeGen() = 0;
    void print(int level);
    virtual Node getNode() = 0;
    ~Expression();
};

class Prohead : public Expression
{
public:
    Token *name;

    Prohead() = default;
    Prohead(int line, Token *using_name);
    Prohead(Token *using_name);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Prohead();
};

class Expressions : public Expression
{
public:
    std::list<Expression *> explist;

    Expressions() = default;
    Expressions(int line, Expression *exp);
    Expressions *add(Expression *);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Expressions();
};

class Classbody : public Expression
{
public:
    Expressions *explist;

    Classbody() = default;
    Classbody(int, Expressions *explist);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    Nodes getNodes();
    ~Classbody();
};

class Class : public Expression
{
public:
    Token *name;
    Classbody *classbody;

    Class() = default;
    Class(int, Token *);
    Class(int, Token *, Classbody *);
    llvm::Value *CodeGen() override;
    Class *add(Expression *);
    void print(int level) override;
    Node getNode() override;
    ~Class();
};

class Proclass : public Expression
{
public:
    std::list<Class *> classes;

    Proclass() = default;
    Proclass(int);
    Proclass(int, Class *);
    llvm::Value *CodeGen() override;
    Proclass *add(Class *);
    void print(int level) override;
    Node getNode() override;
    ~Proclass();
};

class Program : public tnode
{
public:
    Prohead *prohead;
    Proclass *proclass;

    Program() = default;
    Program(int line, Prohead *head, Proclass *proclass);
    void print(int level) override;
    Node getNode();
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
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Evaluate();
};

class Number : public Expression
{
public:
    Token *token;
    double val;

    Number() = default;
    Number(int line, Token *token);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Number();
};

class Object : public Expression
{
private:
    Token *token;

public:
    Object() = default;
    Object(int line, Token *token);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Object();
};

class Use : public Expression
{
public:
    Expression *exp;

    Use(int line, Expression *exp);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Use();
};

class Operation : public Expression
{
public:
    Expression *left;
    char op;
    Expression *right;

    Operation() = default;
    Operation(int, Expression *, char, Expression *);
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Operation();
};

class Parameter : public Expression
{
private:
    string name;
    Token *type; // or not

public:
    Parameter();
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Parameter();
};

class Parameters : public Expression
{
private:
public:
    Parameters();
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Parameters();
};

class Function : public Expression
{
public:
    Token *name;
    Token *returntype;
    Parameters *parameters;
    Expressions *explist;

    Function();
    llvm::Value *CodeGen() override;
    void print(int level) override;
    Node getNode() override;
    ~Function();
};

#endif