/*
 * @Author: Outsider
 * @Date: 2022-11-19 14:13:37
 * @LastEditors: Outsider
 * @LastEditTime: 2022-11-19 14:35:17
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/print.h
 */
#include "lib/tree.h"
#include "lib/tree_printer.h"

template <typename N>
auto to_string(const N &node)
{
    std::stringstream ss;
    ss << std::endl;
    ss << node;
    return ss.str();
}

using node = Array_tree_node<int>;
using nodes = Array<node>;