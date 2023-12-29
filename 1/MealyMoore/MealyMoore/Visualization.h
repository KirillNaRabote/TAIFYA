#pragma once

#include <windows.h>
#include <iostream>
#include <fstream>

#include "types.h"

const char VISUALIZATION_ABSENCE_CHAR = '_';
const char INPUT_CHAR = 'x';

std::vector<Transition> FindTransitionByState(const TransitionTable& table, int stateToFind)
{
    std::vector<Transition> foundTransitions = {};

    for (size_t i = 0; i < table.size(); i++) 
    {
        for (size_t j = 0; j < table[i].size(); j++) 
        {
            if (table[i][j].state == stateToFind) 
            {
                foundTransitions.push_back(table[i][j]);
            }
        }
    }

    return foundTransitions;
}

void VisualizeMealyGraph(std::ofstream& output, const TransitionTable& table)
{
    for (size_t i = 0; i < table.size(); i++)
    {
        for (size_t j = 0; j < table[0].size(); j++)
        {
            if (table[i][j].state == NUMBER_SYMBOLIZING_ABSENCE)
            {
                continue;
            }

            std::ostringstream msg;
            msg << "    " << STATE_CHAR << j;
            msg << " -> " << STATE_CHAR << table[i][j].state << " [label=\"" << INPUT_CHAR << i + 1;
            if (table[i][j].outputSymbol != NUMBER_SYMBOLIZING_ABSENCE)
            {
                msg << OUTPUT_CHAR << table[i][j].outputSymbol;
            }
            else
            {
                msg << VISUALIZATION_ABSENCE_CHAR;
            }
            msg << "\"];" << std::endl;
            output << msg.str();
        }
    }
}

void VisualizeMooreGraph(std::ofstream& output, const TransitionTable& table)
{
    for (size_t i = 1; i < table.size(); i++)
    {
        for (size_t j = 0; j < table[0].size(); j++)
        {
            if (table[i][j].state == NUMBER_SYMBOLIZING_ABSENCE)
            {
                continue;
            }

            std::ostringstream msg;
            msg << "    " << STATE_CHAR << j;

            if (table[0][j].outputSymbol != NUMBER_SYMBOLIZING_ABSENCE)
            {
                msg << OUTPUT_CHAR << table[0][j].outputSymbol;
            }

            msg << " -> " << STATE_CHAR << table[i][j].state;

            if (table[0][table[i][j].state].outputSymbol != NUMBER_SYMBOLIZING_ABSENCE)
            {
                msg << OUTPUT_CHAR << table[0][table[i][j].state].outputSymbol;
            }

            msg << " [label=\"" << INPUT_CHAR << i << "\"];";

            output << msg.str();
        }
    }
}

void VisualizeGraph(const TransitionTable& table, unsigned type)
{
    std::string graphName = "graph";
    std::string typeStr = (type == MEALY_TYPE) ? "Mealy" : "Moore";
    std::string dotFileName = graphName + typeStr + ".dot";
    std::string imageFileName = "graph" + typeStr + ".png";

    std::ofstream outputDotFile(dotFileName);

    outputDotFile << "digraph G {" << std::endl;

    (type == MEALY_TYPE) ? VisualizeMealyGraph(outputDotFile, table) : VisualizeMooreGraph(outputDotFile, table);

    outputDotFile << "}" << std::endl;

    outputDotFile.close();

    std::string systemMsg = "dot -Tpng " + dotFileName + " -o " + imageFileName;
    system(systemMsg.c_str());
    systemMsg = "start " + imageFileName;
    system(systemMsg.c_str());
}