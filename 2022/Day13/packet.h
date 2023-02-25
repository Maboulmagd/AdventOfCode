#ifndef AOC_2022_DAY13_PACKET_H
#define AOC_2022_DAY13_PACKET_H

#include <vector>
#include <cstdint>
#include <istream>
#include <memory>
#include <optional>
#include <algorithm>

namespace AOC2022Day13Packet {
    struct Token final {
        Token* parent_;
        std::vector<Token*> content_;
        std::optional<int32_t> integer_;

        explicit Token(Token* parent, int32_t integer) : parent_(parent), integer_(integer) {}
        explicit Token(Token* parent) : parent_(parent) {}

        // This function needs to be defined first
        friend auto operator<=>(int32_t value, const Token& right) {
            // Base case, won't hit this on very first call here
            if (right.integer_) {
                return value <=> *right.integer_;
            }
            // Comparing empty list with integer
            if (right.content_.empty()) {
                return std::strong_ordering::greater;
            }
            // Comparing an integer against a non-empty list, so recurse
            const auto cmp = value <=> *right.content_[0];
            if (std::is_eq(cmp)) {
                // 2 <=> [2, .... ]
                if (right.content_.size() == 1) {
                    return std::strong_ordering::equal;
                }
                return std::strong_ordering::less;
            }
            return cmp;
        }

        // This function needs to be defined second
        friend auto operator<=>(const Token& left, int32_t value) {
            // Symmetrical
            const auto cmp = value <=> left;
            if (std::is_lt(cmp)) {
                return std::strong_ordering::greater;
            }
            if (std::is_gt(cmp)) {
                return std::strong_ordering::less;
            }
            return std::strong_ordering::equal;
        }

        // I started out by writing this function first, since this is where the chain of execution begins
        friend auto operator<=>(const Token& left, const Token& right) {
            if (left.integer_ && right.integer_) {
                return *left.integer_ <=> *right.integer_;
            }
            if (left.integer_) {
                return *left.integer_ <=> right;
            }
            if (right.integer_) {
                return left <=> *right.integer_;
            }

            //std::lexicographical_compare_three_way()
            auto left_it = left.content_.begin();
            auto right_it = right.content_.begin();

            while (true) {
                if (left_it == left.content_.end() && right_it == right.content_.end()) {
                    return std::strong_ordering::equal;
                }
                if (left_it == left.content_.end()) {
                    return std::strong_ordering::less;// Left side ran out first, so inputs are in the right order, return less
                }
                if (right_it == right.content_.end()) {
                    return std::strong_ordering::greater;
                }

                const auto cmp = **left_it <=> **right_it;// Recurse either on this same function, or the other ones
                if (!std::is_eq(cmp)) {
                    return cmp;
                }

                ++left_it;
                ++right_it;
            }

            return std::strong_ordering::equal;
        }
    };

    struct Packet final {
        std::vector<std::unique_ptr<Token>> tokens_;
        Token* root_ = nullptr;
        
        Packet() = default;
        explicit Packet(const int32_t divider) {
            root_ = NewListToken(nullptr);
            NewIntegerToken(NewListToken(root_), divider);
        }

        auto operator<=>(const Packet& packet) const {
            return *root_ <=> *packet.root_;
        }

        Token* NewIntegerToken(Token* parent, const int32_t integer) {
            tokens_.push_back(std::make_unique<Token>(parent, integer));
            if (parent != nullptr) {
                parent->content_.push_back(tokens_.back().get());
            }
            return tokens_.back().get();
        }

        Token* NewListToken(Token* parent) {
            tokens_.push_back(std::make_unique<Token>(parent));
            if (parent != nullptr) {
                parent->content_.push_back(tokens_.back().get());
            }
            return tokens_.back().get();
        }

        friend std::istream& operator>>(std::istream& s, Packet& packet) {
            Token* current = nullptr;

            while (s) {
                if (std::isdigit(s.peek())) {
                    int32_t integer;
                    s >> integer;

                    packet.NewIntegerToken(current, integer);
                }
                else if (s.peek() == '[') {
                    s.get();// Extract '['

                    Token* next = packet.NewListToken(current);
                    if (current == nullptr) {
                        packet.root_ = next;
                    }
                    current = next;
                }
                else if (s.peek() == ']') {
                    s.get();// Extract '['

                    current = current->parent_;
                    if (current == nullptr) {// We are at outer-most list, and we've reached the end
                        break;
                    }
                }
                else {
                    s.get();// Extracts the comma, or ',' character
                }
            }

            return s;
        }
    };
}

#endif //AOC_2022_DAY13_PACKET_H
