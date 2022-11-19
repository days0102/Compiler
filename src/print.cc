/*
 * @Author: Outsider
 * @Date: 2022-11-19 14:35:45
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-19 14:53:57
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
        Nodes::build_array("u_1", "u_2")};

    return n;
}

Node Proclass::getNode()
{
    Node n("class");
    n = {
        "class",
        Nodes::build_array("c_1", "c_2")};

    return n;
}