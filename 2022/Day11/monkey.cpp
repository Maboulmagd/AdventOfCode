#include "monkey.h"

using namespace AOC2022Day11Monkey;

uint32_t Monkey::InspectItem(const int item_index) {
    assert(item_index >= 0 && item_index < items_and_worry_level_.size());

    uint32_t item_worry_level = items_and_worry_level_[item_index];

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


    // Update item's worry level
//    switch (operation_.operator_) {
//        case '+':
//            if (operation_operand_is_old_) item_worry_level += item_worry_level;
//            else { item_worry_level += operation_.operand_; }
//            break;
//        case '-':
//            if (operation_operand_is_old_) item_worry_level -= item_worry_level;
//            else { item_worry_level -= operation_.operand_; }
//            break;
//        case '*':
//            if (operation_operand_is_old_) item_worry_level *= item_worry_level;
//            else { item_worry_level *= operation_.operand_; }
//            break;
//        case '/':
//            if (operation_operand_is_old_) item_worry_level /= item_worry_level;
//            else { item_worry_level /= operation_.operand_; }
//            break;
//        default:
//            throw std::runtime_error("Invalid operation_.operator_");
//    }

    return item_worry_level;
}

uint32_t Monkey::TestItem(const int item_index) {
    assert(item_index >= 0 && item_index < items_and_worry_level_.size());

    const uint32_t item_worry_level = items_and_worry_level_.at(item_index);
    uint32_t monkey_to_throw_to = if_false_monkey_;

    // We know the operation is division
    const int32_t operand = test_.operand_;
    if (item_worry_level % operand == 0) {
        monkey_to_throw_to = if_true_monkey_;
    }

    return monkey_to_throw_to;
}