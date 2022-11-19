#pragma once

#include <iostream>

#include "array.h"
#include "forward_list.h"
#include "tree_printer.h"

template<typename T, typename C>
class Base_tree_node {
    private:
        T value_;
        C children_;
    public:
        Base_tree_node(T data) :value_(data) {}
        Base_tree_node(T data, C&& children) :value_(data), children_(std::move(children)) {}
        Base_tree_node() = default;

        Base_tree_node(const Base_tree_node&) = delete;
        Base_tree_node& operator=(const Base_tree_node&) = delete;

        Base_tree_node(Base_tree_node&& o) :value_(std::move(o.value_)), children_(std::move(o.children_)) {}
        Base_tree_node& operator=(Base_tree_node&& o) {
            std::swap(value_, o.value_);
            std::swap(children_, o.children_);
            return *this;
        }

        T value() const { return value_; }
        const C& children() const { return children_; }
        C& children() { return children_; }
};

template<typename T>
class Array_tree_node : public Base_tree_node<T, Array<Array_tree_node<T>>> {
    public:
        using Base = Base_tree_node<T, Array<Array_tree_node<T>>>;
        Array_tree_node(T data, Array<Array_tree_node>&& children) :Base(data, std::move(children)) {}
        Array_tree_node(T data) :Base(data) {}
        Array_tree_node() = default;
};

template<typename T>
class Forward_list_tree_node : public Base_tree_node<T, Forward_list<Forward_list_tree_node<T>>> {
    public:
        using Base = Base_tree_node<T, Forward_list<Forward_list_tree_node<T>>>;
        Forward_list_tree_node(T data, Forward_list<Forward_list_tree_node>&& children) :Base(data, std::move(children)) {}
        Forward_list_tree_node(T data) :Base(data) {}
        Forward_list_tree_node() = default;
};

template<typename T, typename C>
std::ostream& operator<<(std::ostream& stream, const Base_tree_node<T, C>& node) {
    Tree_printer<Base_tree_node<T, C>>::default_instance().print(node, stream);
    return stream;
}

