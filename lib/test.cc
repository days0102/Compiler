/*
 * @Author: Outsider
 * @Date: 2022-11-18 19:59:37
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-19 14:33:48
 * @Description: In User Settings Edit
 * @FilePath: /compiler/lib/test.cc
 */
#include "tree_printer.h"
#include "tree.h"

template <typename N>
auto to_string(const N &node)
{
        std::stringstream ss;
        ss << std::endl;
        ss << node;
        return ss.str();
}

int main()
{
        using node = Array_tree_node<int>;
        using nodes = Array<node>;
        node n(1);
        n = {1, nodes::build_array(11)};
        std::cout << to_string(n) << std::endl;

        n = {1, nodes::build_array(
                    node(11, nodes::build_array(111, 222, 112, 113)),
                    node(12, nodes::build_array(
                                 121,
                                 node(122, nodes::build_array(1221, 1222, 1223)))),
                    node(13, nodes::build_array(
                                 node(131, nodes::build_array(1311, 1312, 1313)),
                                 node(132, nodes::build_array(1321, 1322)),
                                 133)))};
        std::cout << to_string(n) << std::endl;

        using Node = Array_tree_node<std::string>;
        using Nodes = Array<Node>;

        Node nd("program");
        nd = {"program", Nodes::build_array(
                             Node("class_1", Nodes::build_array("exp1", "exp2")),
                             Node("class_2", Nodes::build_array("exp1", "exp2")))};
        std::cout << to_string(nd) << std::endl;
}