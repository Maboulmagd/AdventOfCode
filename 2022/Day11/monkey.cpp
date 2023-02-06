#include "monkey.h"

using namespace AOC2022Day11Monkey;

int64_t Monkey::InspectItem(const int64_t item) {
    int64_t item_worry_level = item;

    if (operation_.operator_ == '*') {
        if (operation_operand_is_old_) item_worry_level *= item_worry_level;
        else { item_worry_level *= operation_.operand_; }
    }
    else if (operation_.operator_ == '+') {
        if (operation_operand_is_old_) item_worry_level += item_worry_level;
        else { item_worry_level += operation_.operand_; }
    }
    else if (operation_.operator_ == '-') {
        if (operation_operand_is_old_) item_worry_level -= item_worry_level;
        else { item_worry_level -= operation_.operand_; }
    }
    else {
        if (operation_operand_is_old_) item_worry_level /= item_worry_level;
        else { item_worry_level /= operation_.operand_; }
    }

    return item_worry_level;
}

uint32_t Monkey::TestItem(const int64_t item) {
    uint32_t monkey_to_throw_to = if_false_monkey_;

    // We know the operation is division
    const int32_t operand = test_.operand_;
    if (item % operand == 0) {
        monkey_to_throw_to = if_true_monkey_;
    }

    return monkey_to_throw_to;
}