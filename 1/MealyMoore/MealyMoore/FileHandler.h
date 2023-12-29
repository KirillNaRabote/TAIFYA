#pragma once

#include <string>
#include <string>
#include <sstream>
#include <fstream>

#include "types.h"

const char MEALY_SEPARATOR = '/';
const char STATE_CHAR = 's';
const char OUTPUT_CHAR = 'y';
const char ABSENCE_CHAR = '-';

std::vector<Transition> CellsToMealyTable(const std::vector<std::string>& cells)
{
    std::vector<Transition> tableLine;
    int state, outputSymbol;
    std::vector<std::string> parts;
    std::string part;

    for (auto cell : cells)
    {
        std::istringstream cellStream(cell);

        while (std::getline(cellStream, part, MEALY_SEPARATOR))
        {
            parts.push_back(part);
        }

        for (auto& p : parts)
        {
            p.erase(remove_if(p.begin(), p.end(), [](char ch) {
                return !isdigit(ch);
                }));
        }

        (parts[0].length() == 0) ? state = -2 : state = stoi(parts[0]);
        (parts[1].length() == 0) ? outputSymbol = -2 : outputSymbol = stoi(parts[1]);

        tableLine.push_back({ state, outputSymbol });

        parts.clear();
    }

    return tableLine;
}

std::vector<Transition> CellsToMooreTable(std::vector<std::string> cells, bool isFirstLine)
{
    std::vector<Transition> tableLine;
    int state, outputSymbol;

    for (auto cell : cells)
    {
        std::string c = cell;

        c.erase(std::remove_if(c.begin(), c.end(), [](char ch) {
            return !isdigit(ch);
            }));

        if (isFirstLine)
        {
            state = -1;
            (c.length() == 0) ? outputSymbol = -2 : outputSymbol = stoi(c);
        }
        else
        {
            outputSymbol = -1;
            (c.length() == 0) ? state = -2 : state = stoi(c);
        }

        tableLine.push_back({ state, outputSymbol });
    }

    return tableLine;
}

ReadFuncReturnableValue ReadTransitionTableFromFile(std::ifstream& input)
{
    unsigned numOfColumns, numOfRows, type;

    input >> numOfColumns >> numOfRows >> type;

    std::vector<std::string> cells;
    std::string cell;
    std::string line;

    getline(input, line);

    TransitionTable table;
    std::vector<Transition> tableLine;

    bool mooreFirstLine = true;

    while (getline(input, line))
    {
        std::istringstream lineStream(line);

        while (lineStream >> cell)
        {
            cells.push_back(cell);
        }

        tableLine = (type == 1) ? CellsToMealyTable(cells) : CellsToMooreTable(cells, mooreFirstLine);

        table.push_back(tableLine);

        cells.clear();
        tableLine.clear();

        mooreFirstLine = false;
    }

    return { table, type };
}

void PrintTransitionTableToFile(std::ofstream& output, const TransitionTable& table, unsigned type)
{
    const int distanceBetweenCellsMoore = 8;
    const int distanceBetweenCellsMealy = 12;
    output << table[0].size() << " ";
    (type == 1) ? output << table.size() : output << table.size() - 1;
    output << " " << type << std::endl;

    for (const auto& line : table)
    {
        for (const auto& transition : line)
        {
            int freeDistanceBetweenCells;
            (type == 1) ? freeDistanceBetweenCells = distanceBetweenCellsMealy : freeDistanceBetweenCells = distanceBetweenCellsMoore;

            if (transition.state != -1)
            {
                freeDistanceBetweenCells--;
                if (transition.state == -2)
                {
                    output << ABSENCE_CHAR;
                }
                else
                {
                    output << STATE_CHAR << transition.state;
                    freeDistanceBetweenCells -= (transition.state != 0) ? static_cast<int>(log10(abs(transition.state))) + 1 : 1;
                }

                if (type == 2)
                {
                    output << " ";
                    freeDistanceBetweenCells--;
                }
            }
            if (transition.outputSymbol != -1)
            {
                if (type == 1)
                {
                    output << MEALY_SEPARATOR;
                    freeDistanceBetweenCells--;
                }
                freeDistanceBetweenCells--;
                if (transition.outputSymbol == -2)
                {
                    output << ABSENCE_CHAR;
                }
                else
                {
                    output << OUTPUT_CHAR << transition.outputSymbol;
                    freeDistanceBetweenCells -= (transition.outputSymbol != 0) ? static_cast<int>(log10(abs(transition.outputSymbol))) + 1 : 1;
                }
            }
            for (int i = 1; i <= freeDistanceBetweenCells; i++)
            {
                output << " ";
            }
        }
        output << std::endl;
    }
}