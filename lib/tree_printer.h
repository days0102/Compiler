#pragma once

#include <iostream>
#include <sstream>

#include "array.h"
#include "forward_list.h"
#include "string_utils.h"

class Tree_printer_base {
    public:
        struct Siblings;

        class Printed_node {
            private:
                friend class Siblings;
                const std::string label_;
                const int label_width_;
                const int label_half_width_;
                int position_;

                Printed_node(std::string&& label, size_t width, Siblings* siblings)
                    :label_(label), label_width_(width), label_half_width_(width / 2), position_(0), next_(nullptr), siblings_(siblings)
                {}
                Printed_node(Printed_node&&) = delete;
                Printed_node* next_;
            public:
                Siblings* const siblings_;

                const std::string label() const { return label_; }

                int position() { return position_; }
                void position(int p) { position_ = p; }

                int center() const { return position_ + label_half_width_; }
                void center(int c) { position_ = c - label_half_width_; }

                void shift(int s) { position_ += s; }
                int border() const { return position_ + label_width_ + 2; }
                size_t label_width() const { return label_width_; }
                Printed_node* next() { return next_; }
        };

        class Siblings {
            private:
                Printed_node* head_;
                Printed_node* tail_;
            public:
                Printed_node* const parent_;
                Siblings* next_;

                Siblings(Printed_node* parent) 
                    :head_(nullptr), tail_(nullptr), parent_(parent), next_(nullptr)
                {}
                ~Siblings() {
                    for (Printed_node* node = head_; node; ) {
                        auto previous = node;
                        node = node->next_;
                        delete previous;
                    }
                }
                Siblings(Siblings&& o) = delete;
                Printed_node* head() { return head_; }
                auto add_node(std::string&& label, size_t width) {
                    Printed_node* node = new Printed_node(std::move(label), width, this);
                    if (tail_) tail_->next_ = node;
                    else head_ = node;
                    tail_ = node;
                    return node;
                }
                void fix_positions(Siblings* previous = nullptr);
                template<typename F>
                    void for_each(F func) {
                        for (auto node = head_; node; node = node->next_) {
                            func(node);
                        }
                    }
        };

        using Line = Forward_list<Siblings>;
        using Lines = Forward_list<Line>;

        void print(Lines& lines, std::ostream& stream);
        Forward_list<std::string> compose_text_lines(Lines& lines);
        template<typename A>
            void print(Lines& lines, A&& appender);
};

template<typename N>
class Tree_printer_node_handler {
    public:
        std::string node_to_string(const N& node) {
            std::stringstream ss;
            ss << node.value();
            return ss.str();
        }
        size_t label_width(const N& node, const std::string& label) {
            return label.size();
        }
        template<typename F>
            void iterate_node_children(const N& n, F f) {
                auto& children = n.children();
                for (auto child = children.cbegin(); child != children.cend(); ++child)
                    f(&*child);
            }
        bool node_is_empty(const N& n) {
            return false;
        }
};

template<typename N, typename NH = Tree_printer_node_handler<N>>
class Tree_printer : protected NH, protected Tree_printer_base {
    private:
        Lines compose_lines(const N& node) {
            struct Node_info {
                const N* node_;
                int level_;
                Printed_node* parent_;
                Node_info(const N* node, int level, Printed_node* parent) 
                    :node_(node), level_(level), parent_(parent) 
                {}
            };

            Forward_list<Node_info> queue;
            queue.emplace_back(&node, 1, nullptr);
            int level = 0;
            Lines lines;

            while (!queue.empty()) {
                auto n = queue.pop_front();
                if (n.level_ > level) {
                    lines.emplace_back();
                    level = n.level_;
                }
                auto& line = lines.back();
                if (line.empty())
                    line.emplace_back(n.parent_);
                Siblings* siblings = &line.back();
                if (siblings->parent_ != n.parent_) {
                    line.emplace_back(n.parent_);
                    Siblings* previous = siblings;
                    siblings = &line.back();
                    previous->next_ = siblings;
                }
                if (n.node_) {
                    auto label = NH::node_to_string(n.node_->value());
                    auto label_width = NH::label_width(*n.node_, label);
                    auto printable_node = siblings->add_node(std::move(label), label_width);
                    if (!NH::node_is_empty(*n.node_))
                        NH::iterate_node_children(*n.node_, [&queue, n, printable_node](auto child) {
                            queue.emplace_back(child, n.level_ + 1, printable_node);
                        });
                } else
                    siblings->add_node(" ", 1); // todo add custom empty node rendering to stringifier

            }
            return lines;
        }

    public:
        void print(const N& node, std::ostream& stream) {
            auto lines = compose_lines(node);
            Tree_printer_base::print(lines, stream);
        }

        Forward_list<std::string> compose_text_lines(const N& node) {
            auto lines = compose_lines(node);
            return Tree_printer_base::compose_text_lines(lines);
        }

        static Tree_printer default_instance() {
            static Tree_printer printer;
            return printer;
        }
};

