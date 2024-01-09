#pragma once

#include <algorithm>
#include <set>
#include <fstream>

#include "types.h"

class CNKAtoDKA
{
public:
	static Table ConvertNKAtoDKA(const Table& tableNKA);

private:
	static Transitions CreateTransitionsByEmptySymbols(Table& tableNKA, std::set<std::string>& namesOfChangedVertexes);
	static void AddOutputVertexes(const Table& table, Vertex vertex, std::vector<Vertex>& visitedVertexes);
	static Table CreateDKATable(const Table& tableNKA, const Transitions& transitions, std::vector<Vertex> cellToAdd);
	static void ChangeFinishStates(Table& tableDKA);
	static Table MinimizeDKATable(const Table& tableDKA);

	static bool WereSuchOutputCells(
		const std::vector<std::pair<std::vector<Vertex>, std::vector<std::vector<Vertex>>>>& inputCellOutputCells, 
		const std::vector<std::vector<Vertex>>& outputCells
	);
	static void AddOutputCellsToPair(
		std::vector<std::pair<std::vector<Vertex>, std::vector<std::vector<Vertex>>>>& inputCellOutputCells,
		const std::vector<std::vector<Vertex>>& outputCells,
		std::vector<Vertex> inputCell
	);

	static bool AddInputCellToDKATable(
		Table& tableDKA, 
		std::vector<std::vector<Vertex>>& visitedInputCells, 
		std::vector<Vertex> cell
	);
	static void AddTransitionToDKATable(
		Table& tableDKA, 
		const Table& tableNKA, 
		const std::vector<Vertex>& transitionsOutput, 
		std::vector<Vertex> stateToAddTransition
	);
	static void AddColumnToDKATable(
		Table& tableDKA, 
		const Table& tableNKA, 
		std::vector<std::vector<Vertex>>& visitedInputCells, 
		std::vector<Vertex> cellToAdd, 
		const Transitions& transitions
	);

	static size_t FindRowByOutputSymbol(const Table& table, const std::vector<Vertex>& vertexToFind);
	static size_t FindColumnByInputCell(const Table& table, const std::vector<Vertex>& vertexToFind);
	static size_t FindColumnWhichContainsCell(Table table, std::vector<Vertex> cell);
	static std::vector<Vertex> FindTransitionsByVertex(const Transitions& transitions, const Vertex vertex);

	static bool AreEqualCells(const std::vector<Vertex>& v1, const std::vector<Vertex>& v2);
	static bool AreEqualTables(const Table& t1, const Table& t2);

	static bool IncludeVertex(const std::vector<Vertex>& visitedVertexes, Vertex vertex);
	static bool IncludeVertexByName(const std::vector<Vertex>& visitedVertexes, Vertex vertex);
	static bool IncludeCell(const std::vector<std::vector<Vertex>>& inputCells, const std::vector<Vertex>& cell);

	static bool CompareByName(Vertex v1, Vertex v2);
};