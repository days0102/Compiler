/*
 * @Author: Outsider
 * @Date: 2022-11-19 14:35:45
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-20 09:08:25
 * @Description: In User Settings Edit
 * @FilePath: /compiler/src/print.cc
 */
#include "print.hh"
#include "tree.hh"

Node Program::getNode()
{
    Node n("program");
    n = {
        "program",
        Nodes::build_array(
            this->prohead->getNode(),
            this->proclass->getNode())};

    return n;
}

Node Prohead::getNode()
{
    Node n("using");
    n = {
        "using",
        Nodes::build_array(this->name->str)};

    return n;
}

Node Proclass::getNode()
{
    Array_builder<Nodes, Node> b;

    auto it = this->classes.begin();
    for (; it != this->classes.end(); ++it)
    {
        b.add((*it)->getNode());
    }

    // for (int i = 0; i < 3; i++)
    // {
    //     Array_builder<Nodes, Node> builder;
    //     builder.add("l1", "l2", "l3");
    //     b.add("name", Node("node", builder.build()));
    // }

    Node n("class");
    n = {
        "class",
        b.build()};

    return n;
}

Node Class::getNode()
{
    Node n(this->name->str);
    if (this->classbody == nullptr)
    {
        return n;
    }
    n = {
        this->name->str,
        this->classbody->getNodes()};

    return n;
}
Nodes Classbody::getNodes()
{
    Array_builder<Nodes, Node> b;
    b.add(this->explist->getNode());
    b.add(Node("function"));

    return b.build();
}
Node Classbody::getNode() // unuse
{
    Array_builder<Nodes, Node> b;
    b.add(this->explist->getNode());
    b.add(Node("function"));

    Node n("body");
    n = {
        "",
        b.build()};
    return n;
}

Node Expressions::getNode()
{
    Array_builder<Nodes, Node> b;
    auto it = this->explist.begin();
    for (; it != this->explist.end(); ++it)
    {
        b.add((*it)->getNode());
    }
    Node n("expression");
    n = {
        "expression",
        b.build()};

    return n;
}

Node Evaluate::getNode()
{
    Array_builder<Nodes, Node> builder;
    builder.add(Node(this->left->str));
    builder.add(this->right->getNode());
    Node n("evaluate");
    n = {
        "=",
        builder.build()};
    return n;
}

Node Number::getNode()
{
    Node n(this->token->str);
    return n;
}

Node Object::getNode()
{
    Node n(this->token->str);
    return n;
}

Node Use::getNode()
{
    Array_builder<Nodes, Node> builder;
    builder.add(this->exp->getNode());
    Node n("use");
    n = {
        "use",
        builder.build()};
    return n;
}

Node Operation::getNode()
{
    Array_builder<Nodes, Node> b;
    b.add(this->left->getNode());
    b.add(this->right->getNode());

    Node n(std::string(1, this->op),b.build());
    // n = {
    //     std::string(1, this->op),
    //     b.build()};
    return n;
}

Node Parameter::getNode()
{
    Node n("parameter");
    n = {
        "parameter",
        Nodes::build_array("...")};
    return n;
}

Node Parameters::getNode()
{
    Node n("parameters");
    n = {
        "parameters",
        Nodes::build_array("...")};
    return n;
}

Node Function::getNode()
{
    Node n("funtion");
    n = {
        "funtion",
        Nodes::build_array("...")};
    return n;
}