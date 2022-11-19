#pragma once

#include <type_traits>

namespace Collections {

    template<typename T, bool T_is_const>
        class Reverse_iterator {
            private:
                using value_type = std::conditional_t<T_is_const, const T, T>;
                value_type* ptr_;
            public:
                Reverse_iterator(value_type* ptr) :ptr_(ptr) {}
                value_type& operator*() const { return *ptr_; }
                value_type* operator->() const { return ptr_; }
                bool operator==(const Reverse_iterator& o) const { return ptr_ == o.ptr_; }
                bool operator!=(const Reverse_iterator& o) const { return !operator==(o); }
                Reverse_iterator& operator++() { --ptr_; return *this; }
        };

}
