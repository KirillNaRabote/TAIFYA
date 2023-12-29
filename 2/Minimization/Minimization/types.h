#pragma once
#include <vector>

const int NUMBER_SYMBOLIZING_ABSENCE = -2;
const int NUMBER_SYMBOLIZING_EMPTINESS = -1;
const unsigned MEALY_TYPE = 1;
const unsigned MOORE_TYPE = 2;

struct Transition
{
    int state;
    int outputSymbol;

    bool operator<(const Transition& other) const {
        return state < other.state || (state == other.state && outputSymbol < other.outputSymbol);
    }
};

using TransitionTable = std::vector<std::vector<Transition>>;

struct ReadFuncReturnableValue
{
    TransitionTable table;
    unsigned type;
};