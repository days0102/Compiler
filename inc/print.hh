/*
 * @Author: Outsider
 * @Date: 2022-11-19 14:13:37
 * @LastEditors: Outsider
 * @LastEditTime: 2022-12-07 19:51:55
 * @Description: In User Settings Edit
 * @FilePath: /compiler/inc/print.hh
 */
#ifndef PRINT_H
#define PRINT_H

#include "lib/tree.h"
#include "lib/tree_printer.h"

template <typename N>
auto to_string(const N &node)
{ // 将 AST转换成字符串
    std::stringstream ss;
    ss << std::endl;
    ss << node;
    return ss.str();
}
// 详细内容在 lib/ 目录下
using Node = Array_tree_node<std::string>;
using Nodes = Array<Node>;

#endif