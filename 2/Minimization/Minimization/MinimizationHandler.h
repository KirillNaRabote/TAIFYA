#pragma once

#include <set>
#include <iostream>
#include <algorithm>

#include "types.h"

void AddReachableVert(const TransitionTable& table, std::set<unsigned>& reachableVertices, int vert, unsigned type)
{
	reachableVertices.insert(vert);
	size_t row;
	(type == MEALY_TYPE) ? row = 0 : row = 1;

	for (row; row < table.size(); row++)
	{
		int newVert = table[row][vert].state;
		if (newVert != NUMBER_SYMBOLIZING_ABSENCE
			&& reachableVertices.find(newVert) == reachableVertices.end())
		{
			AddReachableVert(table, reachableVertices, newVert, type);
		}
	}
}

TransitionTable RemoveUnreachableVertices(const TransitionTable& table, unsigned type)
{
	std::set<unsigned> reachableVertices;
	int initialVert = 0;
	AddReachableVert(table, reachableVertices, initialVert, type);

	size_t row;
	(type == MEALY_TYPE) ? row = 0 : row = 1;

	std::set<int> allVertices;
	for (size_t i = 0; i < table[0].size(); i++)
	{
		allVertices.insert(i);
	}

	std::set<int> unreachableVertices;
	
	TransitionTable tableWithoutUnreachableVertices(table.size(), std::vector<Transition>(reachableVertices.size()));
	std::set_difference(allVertices.begin(), allVertices.end(), reachableVertices.begin(), reachableVertices.end(),
		std::inserter(unreachableVertices, unreachableVertices.begin()));

	size_t col = 0;
	for (std::set<unsigned>::iterator itReachable = reachableVertices.begin(); itReachable != reachableVertices.end(); itReachable++, col++)
	{
		for (row = 0; row < tableWithoutUnreachableVertices.size(); row++)
		{
			tableWithoutUnreachableVertices[row][col] = table[row][*itReachable];
			int numberOfDeletedVertices = 0;
			for (auto itUnreachable = unreachableVertices.begin(); itUnreachable != unreachableVertices.end(); itUnreachable++)
			{
				if (tableWithoutUnreachableVertices[row][col].state > *itUnreachable)
				{
					numberOfDeletedVertices++;
				}
			}

			if (type == MOORE_TYPE && row == 0) continue;
			tableWithoutUnreachableVertices[row][col].state -= numberOfDeletedVertices;
		}
	}

	return tableWithoutUnreachableVertices;
}

//добавляет вершину в структуру следующего типа
//A0 = {0; 1; 4; 5; } : [1; 1; ]
//A0 - название
//{} - вершины с одинаковыми выходными сигналами
//[] - выходные сигналы / ?переходы?

std::vector<std::pair<std::set<unsigned>, std::vector<int>>> SplitVerticesIntoIntermediateSets(const TransitionTable& table, unsigned type)
{
	std::vector<std::pair<std::set<unsigned>, std::vector<int>>> pairsVertecsOutputSymbols;

	for (size_t col = 0; col < table[0].size(); col++)
	{
		std::vector<int> columnOutputSymbols;

		if (type == MEALY_TYPE)
		{
			for (size_t row = 0; row < table.size(); row++)
			{
				columnOutputSymbols.push_back(table[row][col].outputSymbol);
			}
		}
		else
		{
			columnOutputSymbols.push_back(table[0][col].outputSymbol);
		}

		bool found = false;
		for (size_t pairIndex = 0; pairIndex < pairsVertecsOutputSymbols.size(); pairIndex++)
		{
			if (pairsVertecsOutputSymbols[pairIndex].second == columnOutputSymbols)
			{
				found = true;
				pairsVertecsOutputSymbols[pairIndex].first.insert(col);
			}
		}

		if (!found)
		{
			std::set<unsigned> s;
			s.insert(col);
			pairsVertecsOutputSymbols.push_back(std::make_pair(s, columnOutputSymbols));
		}
	}

	return pairsVertecsOutputSymbols;
}

std::vector<std::pair<std::set<unsigned>, std::vector<int>>> SplitIntermediateVerticesIntoIntermediateSets(
	const TransitionTable& table, 
	const std::vector<std::pair<std::set<unsigned>, std::vector<int>>>& intermediatePairs
)
{
	std::vector<std::pair<std::set<unsigned>, std::vector<int>>> newPairs;

	for (size_t col = 0; col < table[0].size(); col++)
	{
		std::vector<int> columnOutputStates;

		for (size_t row = 1; row < table.size(); row++)
		{
			columnOutputStates.push_back(table[row][col].state);
		}

		bool found = false;
		for (size_t pairIndex = 0; pairIndex < newPairs.size(); pairIndex++)
		{
			if (newPairs[pairIndex].second == columnOutputStates)
			{
				for (size_t oldPairIndex = 0; oldPairIndex < intermediatePairs.size(); oldPairIndex++)
				{
					auto it = intermediatePairs[oldPairIndex].first.find(table[0][col].state);
					if (it != intermediatePairs[oldPairIndex].first.end())
					{
						std::set<unsigned> tSet = newPairs[pairIndex].first;
						tSet.insert(table[0][col].state);
						if (std::includes(intermediatePairs[oldPairIndex].first.begin(), intermediatePairs[oldPairIndex].first.end(),
							tSet.begin(), tSet.end()))
						{
							found = true;
							newPairs[pairIndex].first.insert(table[0][col].state);
							break;
						}
					}
				}
			}
		}

		if (!found)
		{
			std::set<unsigned> s;
			s.insert(table[0][col].state);
			newPairs.push_back(std::make_pair(s, columnOutputStates));
		}
	}

	return newPairs;
}

bool AreEqualPairs(
	const std::vector<std::pair<std::set<unsigned>, std::vector<int>>>& first, 
	const std::vector<std::pair<std::set<unsigned>, std::vector<int>>>& second
)
{
	if (first.size() != second.size())
	{
		return false;
	}

	for (size_t pairIndex = 0; pairIndex != first.size(); pairIndex++)
	{
		if (first[0].first != second[0].first)
		{
			return false;
		}
	}

	return true;
}

int FindIntermediateVertInPairs(const std::vector<std::pair<std::set<unsigned>, std::vector<int>>>& intermediatePairs, int vert)
{
	for (size_t pairIndex = 0; pairIndex < intermediatePairs.size(); pairIndex++)
	{
		auto it = intermediatePairs[pairIndex].first.find(vert);
		if (it != intermediatePairs[pairIndex].first.end())
		{
			return pairIndex;
		}
	}

	return NUMBER_SYMBOLIZING_ABSENCE;
}

TransitionTable CreateIntermediateTable(
	const TransitionTable& originalTable, 
	const std::vector<std::pair<std::set<unsigned>, std::vector<int>>>& intermediatePairs,
	unsigned type
)
{
	std::pair<size_t, size_t> intermediateTableSize;
	intermediateTableSize.second = originalTable[0].size();
	(type == MEALY_TYPE) 
		? intermediateTableSize.first = originalTable.size() + 1 
		: intermediateTableSize.first = originalTable.size();

	TransitionTable intermediateTable(intermediateTableSize.first, std::vector<Transition>(intermediateTableSize.second));
	size_t column = 0;

	for (size_t pairIndex = 0; pairIndex < intermediatePairs.size(); pairIndex++)
	{
		for (auto vertNum : intermediatePairs[pairIndex].first)
		{
			intermediateTable[0][column].state = vertNum;
			intermediateTable[0][column].outputSymbol = NUMBER_SYMBOLIZING_ABSENCE;
			column++;
		}
	}

	for (size_t col = 0; col < intermediateTableSize.second; col++)
	{
		for (size_t row = 1; row < intermediateTableSize.first; row++)
		{
			int numVertToFound;
			(type == MEALY_TYPE)
				? numVertToFound = originalTable[row - 1][intermediateTable[0][col].state].state
				: numVertToFound = originalTable[row][intermediateTable[0][col].state].state;

			intermediateTable[row][col].state = FindIntermediateVertInPairs(intermediatePairs, numVertToFound);
			intermediateTable[row][col].outputSymbol = NUMBER_SYMBOLIZING_ABSENCE;
		}
	}

	return intermediateTable;
}

TransitionTable CreateFinalTableFromIntermediate(
	const TransitionTable& originalTable,
	const TransitionTable& intermediateTable,
	const std::vector<std::pair<std::set<unsigned>, std::vector<int>>>& intermediatePairs,
	unsigned type
)
{
	std::pair<size_t, size_t> finalTableSize;
	finalTableSize.second = intermediatePairs.size();
	(type == MEALY_TYPE) ? finalTableSize.first = intermediateTable.size() - 1 : finalTableSize.first = intermediateTable.size();

	TransitionTable finalTable(finalTableSize.first, std::vector<Transition>(finalTableSize.second));

	for (size_t col = 0; col < finalTableSize.second; col++)
	{
		for (size_t row = 0; row < finalTableSize.first; row++)
		{
			int vertInOriginalTable = *intermediatePairs[col].first.begin();

			if (type == MOORE_TYPE && row == 0)
			{
				finalTable[0][col].state = NUMBER_SYMBOLIZING_EMPTINESS;
				finalTable[0][col].outputSymbol = originalTable[0][vertInOriginalTable].outputSymbol;
				continue;
			}

			for (size_t colInterTable = 0; colInterTable < intermediateTable[0].size(); colInterTable++)
			{
				if (intermediateTable[0][colInterTable].state == vertInOriginalTable)
				{
					(type == MEALY_TYPE) 
						? finalTable[row][col].state = intermediateTable[row + 1][colInterTable].state 
						: finalTable[row][col].state = intermediateTable[row][colInterTable].state;
				}
			}

			(type == MEALY_TYPE)
				? finalTable[row][col].outputSymbol = originalTable[row][vertInOriginalTable].outputSymbol
				: finalTable[row][col].outputSymbol = NUMBER_SYMBOLIZING_EMPTINESS;
		}
	}

	return finalTable;
}

TransitionTable MinimizeTable(const TransitionTable& table, unsigned type)
{
	TransitionTable minimizedTable = RemoveUnreachableVertices(table, type);

	/*for (size_t row = 0; row < minimizedTable.size(); row++)
	{
		for (size_t col = 0; col < minimizedTable[0].size(); col++)
		{
			std::cout << minimizedTable[row][col].state << " " << minimizedTable[row][col].outputSymbol << ";";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	std::vector<std::pair<std::set<unsigned>, std::vector<int>>> oldPairsVertecsOutputSymbols;
	std::vector<std::pair<std::set<unsigned>, std::vector<int>>> newPairsVertecsOutputSymbols = SplitVerticesIntoIntermediateSets(minimizedTable, type);

	/*for (auto p : newPairsVertecsOutputSymbols)
	{
		std::cout << "[";
		for (auto vert : p.first)
		{
			std::cout << vert << ", ";
		}
		std::cout << "] : {";
		for (auto outp : p.second)
		{
			std::cout << outp << ", ";
		}
		std::cout << "}" << std::endl;
	}*/

	TransitionTable intermediateTable = CreateIntermediateTable(minimizedTable, newPairsVertecsOutputSymbols, type);

	/*for (size_t row = 0; row < intermediateTable.size(); row++)
	{
		for (size_t col = 0; col < intermediateTable[0].size(); col++)
		{
			std::cout << intermediateTable[row][col].state << " " << intermediateTable[row][col].outputSymbol << ";";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	/*for (size_t pairIndex = 0; pairIndex < newPairsVertecsOutputSymbols.size(); pairIndex++)
	{
		std::cout << "A" << pairIndex << " = {";
		for (auto vert : newPairsVertecsOutputSymbols[pairIndex].first)
		{
			std::cout << vert << "; ";
		}
		std::cout << "} : [";
		for (auto outputSymbol : newPairsVertecsOutputSymbols[pairIndex].second)
		{
			std::cout << outputSymbol << "; ";
		}
		std::cout << "]" << std::endl;
	}*/

	while (!AreEqualPairs(oldPairsVertecsOutputSymbols, newPairsVertecsOutputSymbols))
	{
		oldPairsVertecsOutputSymbols = newPairsVertecsOutputSymbols;
		newPairsVertecsOutputSymbols = SplitIntermediateVerticesIntoIntermediateSets(intermediateTable, oldPairsVertecsOutputSymbols);
		/*for (size_t pairIndex = 0; pairIndex < newPairsVertecsOutputSymbols.size(); pairIndex++)
		{
			std::cout << "A" << pairIndex << " = {";
			for (auto vert : newPairsVertecsOutputSymbols[pairIndex].first)
			{
				std::cout << vert << "; ";
			}
			std::cout << "} : [";
			for (auto outputSymbol : newPairsVertecsOutputSymbols[pairIndex].second)
			{
				std::cout << outputSymbol << "; ";
			}
			std::cout << "]" << std::endl;
		}*/
		intermediateTable = CreateIntermediateTable(minimizedTable, newPairsVertecsOutputSymbols, type);
	}

	minimizedTable = CreateFinalTableFromIntermediate(minimizedTable, intermediateTable, newPairsVertecsOutputSymbols, type);

	/*for (size_t row = 0; row < intermediateTable.size(); row++)
	{
		for (size_t col = 0; col < intermediateTable[0].size(); col++)
		{
			std::cout << intermediateTable[row][col].state << " " << intermediateTable[row][col].outputSymbol << ";";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	/*for (size_t pairIndex = 0; pairIndex < newPairsVertecsOutputSymbols.size(); pairIndex++)
	{
		std::cout << "A" << pairIndex << " = {";
		for (auto vert : newPairsVertecsOutputSymbols[pairIndex].first)
		{
			std::cout << vert << "; ";
		}
		std::cout << "} : [";
		for (auto outputSymbol : newPairsVertecsOutputSymbols[pairIndex].second)
		{
			std::cout << outputSymbol << "; ";
		}
		std::cout << "]" << std::endl;
	}*/

	/*for (size_t row = 0; row < minimizedTable.size(); row++)
	{
		for (size_t col = 0; col < minimizedTable[0].size(); col++)
		{
			std::cout << minimizedTable[row][col].state << " " << minimizedTable[row][col].outputSymbol << ";";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;*/

	return minimizedTable;
}