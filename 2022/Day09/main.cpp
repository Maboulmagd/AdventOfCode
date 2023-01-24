#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <ranges>
#include <algorithm>
#include <cassert>
#include <unordered_set>

#include "motion.h"
#include "position.h"

using namespace AOC2022Day09Motion;
using namespace AOC2022Day09Position;

void MoveHead(Position& head, const Direction& direction) {
    switch (direction) {
        case Direction::UP:
            --head.row;
            break;
        case Direction::DOWN:
            ++head.row;
            break;
        case Direction::LEFT:
            --head.col;
            break;
        case Direction::RIGHT:
            ++head.col;
            break;
    }
}

void MoveTail(Position& tail, const Position& head) {
    const int32_t tail_row = tail.row;
    const int32_t tail_col = tail.col;

    const int32_t head_row = head.row;
    const int32_t head_col = head.col;

    // If head is ever 2 steps directly up, down, left, or right from tail, move tail accordingly
    if (head_row == tail_row - 2) {// head is 2 steps up
        if (head_col == tail_col - 1) {
            --tail.row;
            --tail.col;
        }
        else if (head_col == tail_col + 1) {
            --tail.row;
            ++tail.col;
        }
        else {
            --tail.row;
        }
    }
    else if (head_row == tail_row + 2) {// head is 2 steps down
        if (head_col == tail_col - 1) {
            ++tail.row;
            --tail.col;
        }
        else if (head_col == tail_col + 1) {
            ++tail.row;
            ++tail.col;
        }
        else {
            ++tail.row;
        }
    }
    else if (head_col == tail_col + 2) {// head is 2 steps to the right
        if (head_row == tail_row - 1) {
            --tail.row;
            ++tail.col;
        }
        else if (head_row == tail_row + 1) {
            ++tail.row;
            ++tail.col;
        }
        else {
            ++tail.col;
        }
    }
    else if (head_col == tail_col - 2) {// head is 2 steps to the left
        if (head_row == tail_row - 1) {
            --tail.row;
            --tail.col;
        }
        else if (head_row == tail_row + 1) {
            ++tail.row;
            --tail.col;
        }
        else {
            --tail.col;
        }
    }
}

std::size_t UniquePositionsVisited(const std::vector<Motion>& motions) {
    std::unordered_set<Position> tail_positions_visited;

    Position head(0, 0);
    Position tail(0, 0);

    tail_positions_visited.insert(tail);// Add tail's origin as a visited position

    const auto parse_motion = [&head, &tail, &tail_positions_visited](const Motion& motion) {
        for (const std::weakly_incrementable auto step : std::views::iota(0uz, motion.steps_)) {
            MoveHead(head, motion.direction_);
            MoveTail(tail, head);

            tail_positions_visited.insert(tail);
        }
    };

    std::ranges::for_each(motions, parse_motion);
    return tail_positions_visited.size();
}

int ParseAndRun(const std::string& path) {
    std::fstream file_stream(path);

    if (!file_stream.is_open()) {
        std::cerr << "Failed to open: " << std::quoted(path) << std::endl;
        return 1;
    }

    std::vector<Motion> motions;
    std::ranges::copy(std::views::istream<Motion>(file_stream), std::back_inserter(motions));
    assert(!motions.empty());

    std::cout << UniquePositionsVisited(motions) << std::endl;
    //std::cout << UniquePositionsVisited(motions) << std::endl;

    return 0;
}

int Test() {
    std::vector<Motion> motions = {
            {Direction::RIGHT, 4},
            {Direction::UP, 4},
            {Direction::LEFT, 3},
            {Direction::DOWN, 1},
            {Direction::RIGHT, 4},
            {Direction::DOWN, 1},
            {Direction::LEFT, 5},
            {Direction::RIGHT, 2}
    };

    assert(UniquePositionsVisited(motions) == 13);

    return 0;
}

int main(int argc, char** argv) {
    if (argc == 1) {
        return Test();
    }

    return ParseAndRun(argv[1]);
}