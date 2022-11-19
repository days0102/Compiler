#pragma once

#include <climits>

#include "collections.h"
#include "forward_list.h"

template<typename A, typename T>
class Array_builder {
    private:
        Forward_list<T> list_;
        size_t count_ = 0;
    public:
        template<typename... Args>
            void emplace(Args&&... args) {
                list_.emplace_back(std::forward<Args>(args)...);
                ++count_;
            }
        template<typename TT>
            void add(TT&& t) {
                list_.push_back(std::forward<TT>(t));
                ++count_;
            }
        template<typename TT, typename... Args>
            void add(TT&& t, Args&&... value) {
                list_.push_back(std::forward<TT>(t));
                ++count_;
                add(std::forward<Args>(value)...);
            }
        A build() {
            A a(new T[count_], count_);
            int i = -1;
            for (auto& v : list_) a.ptr_[++i] = std::move(v);
            return a;
        }
        template<typename... Args>
            static A build_array(Args&&... args) {
                Array_builder<A, T> builder;
                builder.add(std::forward<Args>(args)...);
                return builder.build();
            }
};

template<typename T>
class Array {
    private:
        T* ptr_;
        size_t size_;
        Array(T* ptr, size_t size) :ptr_(ptr), size_(size) {}
    public:
        using Builder = Array_builder<Array, T>;
        friend class Array_builder<Array, T>;

        using iterator = T*;
        using const_iterator = T* const;
        using reverse_iterator = Collections::Reverse_iterator<T, false>;
        using const_reverse_iterator = Collections::Reverse_iterator<T, true>;

        Array() :Array(nullptr, 0) {}
        explicit Array(size_t size) :Array(new T[size], size) {}

        Array(size_t size, const T& t) :Array(size) { fill(t); }

        Array(const std::initializer_list<T>& i_list) :Array(i_list.size()) {
            size_t i = -1;
            for (auto& el : i_list)
                ptr_[++i] = el;
        }

        Array(const Array& o) :Array(o.size_) {
            for (size_t i = 0; i < size_; ++i)
                ptr_[i] = o.ptr_[i];
        }
        Array& operator=(const Array& o) {
            delete[] ptr_;
            ptr_ = new T[o.size_];
            size_ = o.size_;
            for (size_t i = 0; i < size_; ++i)
                ptr_[i] = o.ptr_[i];
            return *this;
        }

        Array(Array&& o) :ptr_(o.ptr_), size_(o.size_) {
            o.ptr_ = nullptr;
            o.size_ = 0;
        }
        Array& operator=(Array&& o) {
            std::swap(ptr_, o.ptr_);
            std::swap(size_, o.size_);
            return *this;
        }

        ~Array() { delete[] ptr_; }

        T& operator[](size_t index) { return ptr_[index]; }
        const T& operator[](size_t index) const { return ptr_[index]; }

        template<typename... Args>
            void emplace(int index, Args&&... args) {
                ptr_[index] = T(std::forward<Args>(args)...);
            }

        size_t size() const { return size_; }

        void fill(const T& t) {
            for (auto& e : *this) e = t;
        }

        iterator begin() { return ptr_; }
        iterator end() { return ptr_ + size_; }
        const_iterator cbegin() const { return ptr_; }
        const_iterator cend() const { return ptr_ + size_; }
        reverse_iterator rbegin() { return {ptr_ + size_ - 1}; }
        reverse_iterator rend() { return {ptr_ - 1}; }
        const_reverse_iterator crbegin() const { return {ptr_ + size_ - 1}; }
        const_reverse_iterator crend() const { return {ptr_ - 1}; }

        template<typename... Args>
            static auto build_array(Args&&... args) {
                return Array_builder<Array<T>, T>::build_array(std::forward<Args>(args)...);
            }
};

template<typename T>
bool operator==(const Array<T>& a1, const Array<T>& a2) {
    if (a1.size() != a2.size())
        return false;
    for (size_t i = 0; i < a1.size(); ++i)
        if (a1[i] != a2[i])
            return false;
    return true;
}

template<typename T>
std::ostream& operator<<(std::ostream& stream, const Array<T>& a) {
    stream << "[";
    auto it = a.cbegin();
    if (it != a.cend()) {
        stream << *it;
        for (++it; it != a.cend(); ++it)
            stream << ", " << *it;
    }
    return stream << "]";
}

template<>
class Array<bool> {
    public:
        using Builder = Array_builder<Array, bool>;
        friend class Array_builder<Array, bool>;
        class Reference {
            private:
                friend class Array;
                bool value_;
                unsigned char* char_ptr_;
                int bit_index_;
                void set_index(unsigned char* ptr, size_t index) {
                    char_ptr_ = ptr + index / CHAR_BIT;
                    bit_index_ = index % CHAR_BIT;
                    value_ = *char_ptr_ & (0x80 >> bit_index_);
                }
                Reference() :value_(false), char_ptr_(nullptr), bit_index_(0) {};
            public:
                Reference(bool value) :value_(value), char_ptr_(nullptr), bit_index_(0) {
                }
                Reference(const Reference&) = delete;
                Reference& operator=(const Reference& o) = delete;
                Reference(Reference&&) = delete;
                Reference& operator=(Reference&& o) {
                    if (o.value_)
                        *char_ptr_ |= (0x80 >> bit_index_);
                    else
                        *char_ptr_ &= ~(0x80 >> bit_index_); 
                    return *this;
                }
                operator bool() const {
                    return value_;
                }
        };
    private:
        template<typename R>
        class Base_iterator {
            private:
                friend class Array;
                unsigned char* const ptr_;
                size_t index_;
                Reference reference_;
                Base_iterator(unsigned char* ptr, size_t index) :ptr_(ptr), index_(index) {}
            public:
                void operator++() {
                    ++index_;
                };
                bool operator==(const Base_iterator& o) {
                    return index_ == o.index_;
                }
                bool operator!=(const Base_iterator& o) {
                    return index_ != o.index_;
                }
                R& operator*() {
                    reference_.set_index(ptr_, index_);
                    return reference_;
                }
        };
        size_t actual_size_;
        unsigned char* ptr_;
        size_t size_;
        Reference current_reference_;
    public:
        using iterator = Base_iterator<Reference>;
        using const_iterator = Base_iterator<const Reference>;
        Array() :actual_size_(0), ptr_(nullptr), size_(0) {}
        Array(size_t size);
        Array(size_t size, bool value);
        Array(const Array& o) :actual_size_(o.actual_size_), ptr_(new unsigned char[actual_size_]), size_(o.size_) {
            auto p = ptr_;
            auto p_o = o.ptr_;
            for (; p != ptr_ + actual_size_; *p = *p_o, ++p, ++p_o);
        }
        Array& operator=(const Array& o) {
            auto copy = o;
            std::swap(*this, copy);
            return *this;
        }
        Array(Array&& o) : actual_size_(o.actual_size_), ptr_(o.ptr_), size_(o.size_) {
            o.ptr_ = nullptr;
        }
        Array& operator=(Array&& o) {
            std::swap(actual_size_, o.actual_size_);
            std::swap(ptr_, o.ptr_);
            std::swap(size_, o.size_);
            return *this;
        }
        ~Array() {
            delete[] ptr_;
        }

        Reference& operator[](size_t index) {
            current_reference_.set_index(ptr_, index);
            return current_reference_;
        };

        void fill(bool b) {
            for (auto& e : *this) e = b;
        }

        iterator begin() {
            return iterator(ptr_, 0);
        };
        iterator end() {
            return iterator(ptr_, size_);
        };
        const_iterator cbegin() const {
            return const_iterator(ptr_, 0);
        };
        const_iterator cend() const {
            return const_iterator(ptr_, size_);
        };
        template<typename... Args>
            static Array<bool> build_array(Args&&... args) {
                return Array_builder<Array<bool>, bool>::build_array(std::forward<Args>(args)...);
            }
};

template<>
Array<bool> Array_builder<Array<bool>, bool>::build();

