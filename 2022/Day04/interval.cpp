#include "interval.h"

using namespace AOC2022Day04Interval;

bool Interval::Subsumes(const Interval& other) const {
    return (min <= other.min) && (max >= other.max);
}

bool Interval::Overlaps(const Interval& other) const {
    return Subsumes(other) || ((max >= other.min) && (max <= other.max)) || ((min <= other.max) && (min >= other.min));
}