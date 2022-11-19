#include "tree_printer.h"

#include "box_drawing_chars.h"

void Tree_printer_base::Siblings::fix_positions(Siblings* previous) {
    auto middle = [this]() {
        int middle = head_->center();
        if (head_ != tail_)
            middle = middle + (tail_->center() - middle + 1) / 2;
        return middle;
    };

    auto shift = 0;
    if (previous)
        shift = previous->tail_->border() - head_->position();
    int current_middle = middle();
    if (parent_) 
        shift = std::max(shift, parent_->center() - current_middle);

    if (shift > 0)
        for (auto node = head_; node; node = node->next())
            node->shift(shift);

    if (parent_) {
        if (shift > 0)
            current_middle = middle();
        if (parent_->center() != current_middle) {
            parent_->center(current_middle);

            auto previous = parent_;
            for (auto sibling = parent_->next(); sibling; sibling = sibling->next()) {
                int min_position = previous->border();
                if (min_position > sibling->position())
                    sibling->position(min_position);
                previous = sibling;
            }

            parent_->siblings_->fix_positions();

            auto previous_siblings = parent_->siblings_;
            for (auto siblings = previous_siblings->next_; siblings; siblings = siblings->next_) {
                siblings->fix_positions(previous_siblings);
                previous_siblings = siblings;
            }
        }
    }
}

template<typename S>
class Base_appender {
    protected:
        S stream_;
        int count_;
        Base_appender(S&& stream): stream_(std::forward<S>(stream)), count_(0) {}
    public:
        void operator<<(const char* s) {
            stream_ << s;
            ++count_;
        }
        void operator<<(const std::string& s) {
            stream_ << s;
            count_ += s.size();
        }
        void operator<<(const Tree_printer_base::Printed_node* node) {
            stream_ << node->label();
            count_ += node->label_width();
        }
        void repeat_until(int bound, const char* s) {
            while (count_ < bound)
                *this << s;
        }
        int count() { return count_; }
        void reset() { count_ = 0; }
};

void Tree_printer_base::print(Lines& lines, std::ostream& stream) {
    struct Appender : Base_appender<std::ostream&> {
        Appender(std::ostream& stream) :Base_appender<std::ostream&>(stream) {}
        void new_line() {
            stream_ << std::endl;
        }
    };
    print(lines, Appender(stream));
}

Forward_list<std::string> Tree_printer_base::compose_text_lines(Lines& lines) {
    struct Appender : Base_appender<std::stringstream> {
        Forward_list<std::string> lines_;
        Appender(): Base_appender<std::stringstream>(std::stringstream()) {}
        void new_line() {
            lines_.push_back(stream_.str());
            stream_ = std::stringstream();
        }
    };
    Appender appender;
    print(lines, appender);
    appender.new_line();
    return std::move(appender.lines_);
}

template<typename A>
void Tree_printer_base::print(Lines& lines, A&& appender) {
    for (auto& line : lines) {
        Printed_node* previous = nullptr;
        for (auto& siblings : line) {
            siblings.for_each([&previous](auto node) {
                if (previous)
                    node->position(previous->border());
                previous = node;
            });
        }
    }

    namespace bc = Box_drawing_chars;

    auto do_print = [&lines, &appender]() {
        for (auto line = lines.begin(); line != lines.end(); ++line) {
            if (line != lines.begin()) {
                appender.new_line();
                appender.reset();
                for (auto& siblings : *line) {
                    auto node = siblings.head();

                    auto parent_center = siblings.parent_->center();

                    if (node && node->next()) {
                        appender.repeat_until(node->center(), " ");
                        node = node->next();
                        bool first = true;
                        for (; node; node = node->next()) {
                            if (first) {
                                appender << bc::right_bottom;
                                first = false;
                            } else if (parent_center == appender.count())
                                appender << bc::cross;
                            else
                                appender << bc::bottom_t;
                            auto end = node->center();
                            if (parent_center > appender.count() && parent_center < end) {
                                appender.repeat_until(parent_center, bc::h_line);
                                appender << bc::top_t;
                                appender.repeat_until(end, bc::h_line);
                            } else
                                appender.repeat_until(end, bc::h_line);
                        }
                        appender << bc::left_bottom;
                    } else {
                        appender.repeat_until(parent_center, " ");
                        appender << bc::v_line;
                    }
                }
                appender.new_line();
            }
            appender.reset();
            for (auto& siblings : *line)
                siblings.for_each([&appender](auto node) {
                    appender.repeat_until(node->position(), " ");
                    appender << node;
                });
        }
    };

    auto line = lines.begin();
    ++line;
    for (; line != lines.end(); ++line) {
        Siblings* previous = nullptr;
        for (auto& siblings : *line) {
            siblings.fix_positions(previous);
            previous = &siblings;
        }
    }

    do_print();

}
