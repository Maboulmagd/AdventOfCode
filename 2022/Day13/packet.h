#ifndef AOC_2022_DAY13_PACKET_H
#define AOC_2022_DAY13_PACKET_H

#include <vector>
#include <cstdint>
#include <istream>
#include <memory>
#include <optional>

namespace AOC2022Day13Packet {
    struct Token final {
        Token* parent_;
        std::vector<Token*> content_;
        std::optional<int32_t> integer_;

        explicit Token(Token* parent, const int32_t integer) : parent_(parent), integer_(integer) {}
        explicit Token(Token* parent) : parent_(parent) {}

    };

    struct Packet final {
        std::vector<std::unique_ptr<Token>> tokens_;
        Token* root_ = nullptr;

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
