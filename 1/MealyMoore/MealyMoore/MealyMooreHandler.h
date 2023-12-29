#pragma once

#include <algorithm>

#include "types.h"

bool HasTransition(const std::vector<std::pair<Transition, int>>& vec, const Transition& target) {
    auto it = std::find_if(vec.begin(), vec.end(), [&](const std::pair<Transition, int>& element) {
        return element.first.state == target.state && element.first.outputSymbol == target.outputSymbol;
        });

    return it != vec.end();
}

bool SortByValue(const std::pair<Transition, int>& a, const std::pair<Transition, int>& b) {
    return a.second < b.second;
}

bool CompareTransition(const std::pair<Transition, int>& element, const Transition& target) {
    return element.first.state == target.state && element.first.outputSymbol == target.outputSymbol;
}

bool HasState(const std::vector<std::pair<Transition, int>>& vec, int target) {
    auto it = std::find_if(vec.begin(), vec.end(), [&](const std::pair<Transition, int>& element) {
        return element.first.state == target;
        });

    return it != vec.end();
}

TransitionTable MealyToMoore(const TransitionTable& mealyMachine)
{
    std::vector<std::pair<Transition, int>> mealyToMooreMatchMap;
    int mooreVertexCount = 0;
    TransitionTable mooreMachine;
    std::vector<int> usedVertics = {};

    for (size_t i = 0; i < mealyMachine.size(); i++)
    {
        for (size_t j = 0; j < mealyMachine[0].size(); j++)
        {
            if (!HasTransition(mealyToMooreMatchMap, mealyMachine[i][j]))
            {
                if (mealyMachine[i][j].state != NUMBER_SYMBOLIZING_ABSENCE)
                {
                    auto it = std::find(usedVertics.begin(), usedVertics.end(), j);
                    if (it == usedVertics.end()) {
                        usedVertics.push_back(j);
                    }
                    mealyToMooreMatchMap.push_back(std::make_pair(mealyMachine[i][j], mooreVertexCount++));
                }
            }
        }
    }

    for (auto state : usedVertics)
    {
        if (!HasState(mealyToMooreMatchMap, state))
        {
            Transition t = { state, NUMBER_SYMBOLIZING_ABSENCE };
            mealyToMooreMatchMap.push_back(std::make_pair(t, mooreVertexCount++));
        }
    }

    for (auto it = mealyToMooreMatchMap.begin(); it != mealyToMooreMatchMap.end(); it++) {
        const Transition& key = it->first;
        int value = it->second;
    }

    sort(mealyToMooreMatchMap.begin(), mealyToMooreMatchMap.end(), SortByValue);

    std::vector<Transition> mooreOutputSymbols;

    for (auto it = mealyToMooreMatchMap.begin(); it != mealyToMooreMatchMap.end(); it++) {
        const Transition& key = it->first;
        int value = it->second;

        mooreOutputSymbols.push_back({ NUMBER_SYMBOLIZING_EMPTINESS, key.outputSymbol });
    }

    mooreMachine.push_back(mooreOutputSymbols);
    std::vector<Transition> mooreStateLine;

    for (size_t row = 0; row < mealyMachine.size(); row++)
    {
        for (size_t column = 0; column < mooreMachine[0].size(); column++)
        {
            auto it = std::find_if(mealyToMooreMatchMap.begin(), mealyToMooreMatchMap.end(), [&](const std::pair<Transition, int>& element) {
                return element.second == column;
                });

            Transition matchTransition = mealyMachine[row][it->first.state];

            if (/*matchTransition.outputSymbol != NUMBER_SYMBOLIZING_ABSENCE &&*/ matchTransition.state != NUMBER_SYMBOLIZING_ABSENCE)
            {
                it = std::find_if(mealyToMooreMatchMap.begin(), mealyToMooreMatchMap.end(), [&](const std::pair<Transition, int>& element) {
                    return CompareTransition(element, matchTransition);
                    });

                mooreStateLine.push_back({ it->second, NUMBER_SYMBOLIZING_EMPTINESS });
            }
            else
            {
                mooreStateLine.push_back({ NUMBER_SYMBOLIZING_ABSENCE, NUMBER_SYMBOLIZING_EMPTINESS });
            }

        }
        mooreMachine.push_back(mooreStateLine);
        mooreStateLine.clear();
    }

    return mooreMachine;
}

TransitionTable MooreToMealy(const TransitionTable& mooreMachine)
{
    TransitionTable mealyMachine;
    std::vector<Transition> mealyMachineLine;

    for (size_t i = 1; i < mooreMachine.size(); i++)
    {
        for (size_t j = 0; j < mooreMachine[1].size(); j++)
        {
            (mooreMachine[i][j].state == NUMBER_SYMBOLIZING_ABSENCE)
                ? mealyMachineLine.push_back({ NUMBER_SYMBOLIZING_ABSENCE, NUMBER_SYMBOLIZING_ABSENCE })
                : mealyMachineLine.push_back({ mooreMachine[i][j].state, mooreMachine[0][mooreMachine[i][j].state].outputSymbol });
        }
        mealyMachine.push_back(mealyMachineLine);
        mealyMachineLine.clear();
    }

    return mealyMachine;
}

TransitionTable ConvertTable(const TransitionTable& inputTable, unsigned type)
{
    return (type == MEALY_TYPE) ? MealyToMoore(inputTable) : MooreToMealy(inputTable);
}