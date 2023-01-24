#ifndef AOC_2022_DAY09_MOTION_H
#define AOC_2022_DAY09_MOTION_H

#include <cstdint>
#include <istream>

namespace AOC2022Day09Motion {
    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    struct Motion final {
        Direction direction_;
        uint32_t steps_;

        friend std::istream& operator>>(std::istream& file_stream, Motion& motion) {
            char direction;
            if (file_stream >> direction >> motion.steps_) {
                switch (direction) {
                    case 'U':
                        motion.direction_ = Direction::UP;
                        break;
                    case 'D':
                        motion.direction_ = Direction::DOWN;
                        break;
                    case 'L':
                        motion.direction_ = Direction::LEFT;
                        break;
                    case 'R':
                        motion.direction_ = Direction::RIGHT;
                        break;
                    default:
                        throw std::runtime_error("Cannot parse Motion, invalid direction.");
                }
            }
            return file_stream;
        }
    };
} // AOC2022Day09Motion

#endif //AOC_2022_DAY09_MOTION_H
