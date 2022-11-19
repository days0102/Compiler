#pragma once

#include <algorithm>
#include <iostream>

template<typename T>
class Forward_list {
    private:
        class Node;
        Node* head_;
        Node* tail_;

        void append_node(Node* node) {
            if (tail_) tail_->next_ = node;
            else head_ = node;
            tail_ = node;
        }
        void add_all(const Forward_list& o) {
            for (auto node = o.head_; node; node = node->next_)
                push_back(node->value_);
        }
        void remove_nodes() {
            for (Node* node = head_; node; ) {
                Node* previous = node;
                node = node->next_;
                delete previous;
            }
        }
    public:
        template<bool T_is_const>
            class Iterator;
        using iterator = Iterator<false>;
        using const_iterator = Iterator<true>;

        Forward_list() :head_(nullptr), tail_(nullptr) {};

        Forward_list(const std::initializer_list<T>& i_list) :Forward_list() {
            for (auto& item : i_list)
                push_back(item);
        }

        Forward_list(const Forward_list& o) :Forward_list() {
            add_all(o);
        }
        Forward_list& operator=(const Forward_list& o) {
            clear();
            add_all(o);
            return *this;
        }

        Forward_list(Forward_list&& o) :head_(o.head_), tail_(o.tail_) {
            o.head_ = nullptr;
            o.tail_ = nullptr;
        }
        Forward_list& operator=(Forward_list&& o) {
            std::swap(head_, o.head_);
            std::swap(tail_, o.tail_);
            return *this;
        }

        ~Forward_list() { remove_nodes(); }

        void merge_sort();

        template<typename... Args>
            void emplace_back(Args&&... args) {
                append_node(new Node(std::forward<Args>(args)...));
            }

        template<typename TT>
            void push_back(TT&& value) {
                append_node(new Node(std::forward<TT>(value)));
            }

        T& front() { return head_->value_; }
        T& back() { return tail_->value_; }
        T pop_front() {
            auto node = head_;
            head_ = head_->next_;
            if (!head_)
                tail_ = nullptr;
            auto t = node->value_;
            delete node;
            return t;
        }
        iterator begin() {
            return iterator(head_);
        }
        iterator end() {
            static iterator it(nullptr);
            return it;
        }
        const_iterator cbegin() const {
            return const_iterator(head_);
        }
        const_iterator cend() const {
            static const_iterator it(nullptr);
            return it;
        }
        iterator before_end() {
            return iterator(tail_);
        }
        template<typename F>
            bool remove_first_if(F f) {
                if (empty())
                    return false;

                Node* previous;
                Node* current = head_;
                bool found = false;
                for (; !(found = f(current->value_)) && current != tail_; 
                     previous = current, current = current->next_);
                if (!found)
                    return false;

                if (current == head_) {
                    head_ = current->next_;
                    if (!head_)
                        tail_ = nullptr;
                } else {
                    previous->next_ = current->next_;
                    if (current == tail_) 
                        tail_ = previous;
                }
                delete current;
                return true;
            }
        bool empty() const {
            return head_ == nullptr;
        }
        void clear() {
            remove_nodes();
            head_ = nullptr;
            tail_ = nullptr;
        }
};

template<typename T>
struct Forward_list<T>::Node {
    T value_;
    Node* next_;
    template<typename TT>
        Node(TT&& value) :value_(std::forward<TT>(value)), next_(nullptr) {}
    template<typename... Args>
        Node(Args&&... args) :value_(std::forward<Args>(args)...), next_(nullptr) {}
};

template<bool T_is_true, typename T_true_type, typename T_false_type>
struct Choose_type;

template<typename T_true_type, typename T_false_type>
struct Choose_type<true, T_true_type, T_false_type> {
    using type = T_true_type;
};

template<typename T_true_type, typename T_false_type>
struct Choose_type<false, T_true_type, T_false_type> {
    using type = T_false_type;
};

template<typename T>
template<bool T_is_const>
class Forward_list<T>::Iterator {
    private:
        using node_type = typename Choose_type<T_is_const, const Node, Node>::type;
        using value_type = typename Choose_type<T_is_const, const T, T>::type;
        node_type* node_;
    public:
        Iterator(node_type* node) :node_(node) {}
        bool empty() const { return node_ == nullptr; }
        Iterator& operator++() { 
            node_ = node_->next_; 
            return *this;
        }
        bool operator==(const Iterator& o) const { return node_ == o.node_; }
        bool operator!=(const Iterator& o) const { return !operator==(o); }
        value_type& operator*() const { return node_->value_; }
        value_type* operator->() const { return &operator*(); }
};

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Forward_list<T>& a) {
    stream << "[";
    auto it = a.cbegin();
    if (it != a.cend()) {
        stream << *it;
        for (++it; it != a.cend(); ++it)
            stream << ", " << *it;
    }
    return stream << "]";
}

template<typename T>
void Forward_list<T>::merge_sort() {
    if (!head_ || head_ == tail_)
        return;
    struct List {
        Node* head_;
        size_t length_ = 0;
        List(Node* head, size_t length) :head_(head), length_(length) {}
    };
    List l(head_, 0);
    for (Node* node = l.head_; node; node = node->next_)
        ++l.length_;

    static struct {
        List merge(const List& l1, const List& l2) {
            Node* n1 = l1.head_;
            Node* n2 = l2.head_;
            auto append_node = [&n1, &n2]() {
                Node* n;
                if (n2->value_ < n1->value_) {
                    n = n2; n2 = n2->next_;
                } else {
                    n = n1; n1 = n1->next_;
                }
                return n;
            };
            List l(append_node(), l1.length_ + l2.length_);
            Node* n = l.head_;
            while (n1 && n2)
                n = (n->next_ = append_node());
            if (n1)
                n->next_ = n1;
            else
                n->next_ = n2;
            return l;
        }
        List merge_sort(List& l1) {
            if (l1.length_ > 1) {
                size_t length = 0;
                Node* node = l1.head_;
                for (; length < l1.length_ / 2 - 1; ++length)
                    node = node->next_;
                ++length;
                List l2(node->next_, l1.length_ - length);
                node->next_ = nullptr;
                l1.length_ = length;
                l1 = merge(merge_sort(l1), merge_sort(l2));
            }
            return l1;
        }
    } helper;
    l = helper.merge_sort(l);
    head_ = l.head_;
    for (tail_ = l.head_; tail_->next_; tail_ = tail_->next_);
}
