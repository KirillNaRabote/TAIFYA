#pragma once

#include <fstream>
#include <algorithm>

#include "types.h"

const std::string DELIMETER = ";";

void ReadGrammar(
	std::ifstream& input, 
	Grammar& grammar, 
	VecOfStr& nonTerminalSymbols,
	VecOfStr& terminalSymbols,
	std::string& type)
{
	getline(input, type);

	if (type != RIGHT_GRAMMAR_STR && type != LEFT_GRAMMAR_STR)
	{
		throw std::invalid_argument("The wrong type of grammar");
		return;
	}

	std::regex pattern("[^=|\\-\\s]+");
	std::string str;
	VecOfStr tokens;

	while (getline(input, str))
	{
		auto it = std::sregex_iterator(str.begin(), str.end(), pattern);
		auto end = std::sregex_iterator();

		while (it != end)
		{
			tokens.push_back(it->str());
			++it;
		}

		if (std::find(nonTerminalSymbols.begin(), nonTerminalSymbols.end(), tokens[0]) == nonTerminalSymbols.end())
		{
			nonTerminalSymbols.push_back(tokens[0]);
		}

		RulesForOneState rules;
		OutputSignalNewState signalAndState;
		std::vector<OutputSignalNewState> signalAndStateVec;

		for (size_t index = 1; index < tokens.size(); index++)
		{
			bool rightGrammar;
			bool toPush = false;

			(type == RIGHT_GRAMMAR_STR) ? rightGrammar = true : rightGrammar = false;

			if (!rightGrammar && index % 2 == 0 || rightGrammar && index % 2 == 0)
			{
				toPush = true;
			}

			if (rightGrammar && index % 2 != 0 || !rightGrammar && index % 2 == 0)
			{
				signalAndState.first = tokens[index];
				if (std::find(terminalSymbols.begin(), terminalSymbols.end(), tokens[index]) == terminalSymbols.end())
				{
					terminalSymbols.push_back(tokens[index]);
				}
			}
			else if (rightGrammar && index % 2 == 0 || !rightGrammar && index % 2 != 0)
			{
				signalAndState.second = tokens[index];
				if (std::find(nonTerminalSymbols.begin(), nonTerminalSymbols.end(), tokens[index]) == nonTerminalSymbols.end())
				{
					nonTerminalSymbols.push_back(tokens[index]);
				}
			}

			if (toPush)
			{
				signalAndStateVec.push_back(signalAndState);
			}
		}

		rules.first = tokens[0];
		rules.second = signalAndStateVec;

		grammar.push_back(rules);

		tokens.clear();
	}
}

void PrintTableNKA(std::ostream& output, const TableNKA& table)
{
	for (size_t row = 0; row < table.size(); row++)
	{
		for (size_t column = 0; column < table[0].size(); column++)
		{
			output << table[row][column] << DELIMETER;
		}
		output << "\n";
	}
}