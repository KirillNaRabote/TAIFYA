#include "CNKAtoDKA.h"

std::vector<Vertex> RemoveDuplicates(const std::vector<Vertex>& cell)
{
	std::set<std::string> uniqueNames;
	std::vector<Vertex> uniqueVertices;

	for (const auto& vertex : cell) {
		if (uniqueNames.find(vertex.name) == uniqueNames.end()) {
			uniqueNames.insert(vertex.name);
			uniqueVertices.push_back(vertex);
		}
	}

	return uniqueVertices;
}

bool CNKAtoDKA::CompareByName(Vertex v1, Vertex v2)
{
	return v1.name < v2.name;
}

bool CNKAtoDKA::AreEqualCells(const std::vector<Vertex>& v1, const std::vector<Vertex>& v2)
{
	if (v1.size() != v2.size()) return false;
	for (size_t index = 0; index < v1.size(); index++)
	{
		if (v1[index].isFinish != v2[index].isFinish || v1[index].name != v2[index].name) return false;
	}
	return true;
}

size_t CNKAtoDKA::FindColumnByInputCell(const Table& table, const std::vector<Vertex>& vertexToFind)
{
	for (size_t columnIndex = 1; columnIndex < table[0].size(); columnIndex++)
	{
		if (AreEqualCells(table[0][columnIndex], vertexToFind)) return columnIndex;
	}

	return table[0].size();
}

size_t CNKAtoDKA::FindRowByOutputSymbol(const Table& table, const std::vector<Vertex>& vertexToFind)
{
	for (size_t rowIndex = 1; rowIndex < table.size(); rowIndex++)
	{
		if (AreEqualCells(table[rowIndex][0], vertexToFind)) return rowIndex;
	}
}

bool CNKAtoDKA::IncludeVertex(const std::vector<Vertex>& visitedVertexes, Vertex vertex)
{
	for (const auto& visitedVert : visitedVertexes)
	{
		if (visitedVert.name == vertex.name && visitedVert.isFinish == vertex.isFinish) return true;
	}

	return false;
}

bool CNKAtoDKA::IncludeVertexByName(const std::vector<Vertex>& visitedVertexes, Vertex vertex)
{
	for (const auto& visitedVert : visitedVertexes)
	{
		if (visitedVert.name == vertex.name) return true;
	}

	return false;
}

void CNKAtoDKA::AddOutputVertexes(const Table& table, Vertex vertex, std::vector<Vertex>& visitedVertexes)
{
	if (IncludeVertex(visitedVertexes, vertex)) return;
	
	visitedVertexes.push_back(vertex);

	Vertex emptySymbol = {EMPTY_SYMBOL, false};
	std::vector<Vertex> cell;
	cell.push_back(emptySymbol);

	size_t row = FindRowByOutputSymbol(table, cell);

	cell.clear();
	cell.push_back(vertex);
	size_t column = FindColumnByInputCell(table, cell);

	for (const auto& outVertexes : table[row][column])
	{
		AddOutputVertexes(table, outVertexes, visitedVertexes);
	}
}

Transitions CNKAtoDKA::CreateTransitionsByEmptySymbols(Table& tableNKA, std::set<std::string>& namesOfChangedVertexes)
{
	Transitions transitions;

	for (size_t columnIndex = 1; columnIndex < tableNKA[0].size(); columnIndex++)
	{
		std::vector<Vertex> inputVertexes;
		std::vector<Vertex> visitedVertexes;

		AddOutputVertexes(tableNKA, tableNKA[0][columnIndex][0], visitedVertexes);

		bool isFinish = false;
		for (const auto& visitedVertex : visitedVertexes)
		{
			if (visitedVertex.isFinish == true)
			{
				isFinish = true;
				namesOfChangedVertexes.insert(tableNKA[0][columnIndex][0].name);
			}
		}

		std::sort(visitedVertexes.begin(), visitedVertexes.end(), CompareByName);

		tableNKA[0][columnIndex][0].isFinish = isFinish;

		for (auto& row : tableNKA)
		{
			for (auto& cell : row)
			{
				for (auto& vertex : cell)
				{
					if (namesOfChangedVertexes.find(vertex.name) != namesOfChangedVertexes.end()) vertex.isFinish = isFinish;
				}
			}
		}

		transitions.push_back(std::make_pair(tableNKA[0][columnIndex][0], visitedVertexes));
	}

	return transitions;
}

bool CNKAtoDKA::IncludeCell(const std::vector<std::vector<Vertex>>& inputCells, const std::vector<Vertex>& cell)
{
	for (const auto& inputCell : inputCells)
	{
		if (AreEqualCells(inputCell, cell)) return true;
	}

	return false;
}

bool CNKAtoDKA::AddInputCellToDKATable(Table& tableDKA, std::vector<std::vector<Vertex>>& visitedInputCells, std::vector<Vertex> cell)
{
	if (IncludeCell(visitedInputCells, cell)) return false;

	visitedInputCells.push_back(cell);

	tableDKA[0].push_back(cell);

	for (size_t row = 1; row < tableDKA.size(); row++)
	{
		tableDKA[row].resize(tableDKA[row].size() + 1);
	}

	return true;
}

void CNKAtoDKA::AddTransitionToDKATable(Table& tableDKA, const Table& tableNKA, const std::vector<Vertex>& transitionsOutput, std::vector<Vertex> stateToAddTransition)
{
	std::vector<Vertex> cell;

	for (const auto& vertex : transitionsOutput)
	{
		for (size_t row = 1; row < tableNKA.size(); row++)
		{
			cell.clear();
			if (tableNKA[row][0][0].name == EMPTY_SYMBOL) continue;

			std::vector<Vertex> tempCell;
			tempCell.clear();
			tempCell.push_back(vertex);
			size_t column = FindColumnByInputCell(tableNKA, tempCell);

			cell = tableNKA[row][column];

			tempCell.clear();
			tempCell.push_back(tableNKA[row][0][0]);

			size_t rowDKA = FindRowByOutputSymbol(tableDKA, tempCell);

			size_t columnDKA = FindColumnByInputCell(tableDKA, stateToAddTransition);

			for (auto v : cell)
			{
				tableDKA[rowDKA][columnDKA].push_back(v);
			}

			if (tableDKA[rowDKA][columnDKA].size() != 0)
			{
				std::sort(tableDKA[rowDKA][columnDKA].begin(), tableDKA[rowDKA][columnDKA].end(), CompareByName);
				tableDKA[rowDKA][columnDKA] = RemoveDuplicates(tableDKA[rowDKA][columnDKA]);
			}
		}
	}
}

std::vector<Vertex> CNKAtoDKA::FindTransitionsByVertex(const Transitions& transitions, const Vertex vertex)
{
	for (const auto& transition : transitions)
	{
		if (transition.first.name == vertex.name && transition.first.isFinish == vertex.isFinish) return transition.second;
	}

	std::string msg = "Vertex " + vertex.name + " doesn't go anywhere";
	throw std::invalid_argument(msg);
}

void CNKAtoDKA::AddColumnToDKATable(
	Table& tableDKA, 
	const Table& tableNKA, 
	std::vector<std::vector<Vertex>>& visitedInputCells, 
	std::vector<Vertex> cellToAdd,
	const Transitions& transitions
)
{
	if (!AddInputCellToDKATable(tableDKA, visitedInputCells, cellToAdd)) return;

	std::vector<Vertex> transitionsOuput;
	transitionsOuput.clear();

	for (const auto& vertex : cellToAdd)
	{
		std::vector<Vertex> tempTransitions = FindTransitionsByVertex(transitions, vertex);

		transitionsOuput.insert(transitionsOuput.end(), tempTransitions.begin(), tempTransitions.end());

	}

	transitionsOuput.erase(std::unique(transitionsOuput.begin(), transitionsOuput.end(),
		[](const Vertex& v1, const Vertex& v2) {
			return v1.name == v2.name;
		}),
		transitionsOuput.end());

	std::sort(transitionsOuput.begin(), transitionsOuput.end(), CNKAtoDKA::CompareByName);

	AddTransitionToDKATable(tableDKA, tableNKA, transitionsOuput, cellToAdd);

	size_t column = tableDKA[0].size() - 1;

	for (size_t row = 1; row < tableDKA.size(); row++)
	{
		if (tableDKA[row][column].size() != 0) AddColumnToDKATable(tableDKA, tableNKA, visitedInputCells, tableDKA[row][column], transitions);
	}
}

Table CNKAtoDKA::CreateDKATable(const Table& tableNKA, const Transitions& transitions, std::vector<Vertex> cellToAdd)
{
	Table tableDKA(1, std::vector<std::vector<Vertex>>(1, std::vector<Vertex>(0)));

	for (size_t row = 1; row < tableNKA.size(); row++)
	{
		std::vector<Vertex> cell;
		std::vector<std::vector<Vertex>> line;

		if (tableNKA[row][0][0].name == EMPTY_SYMBOL && tableNKA[row][0][0].isFinish == false) continue;

		line.push_back(tableNKA[row][0]);

		tableDKA.push_back(line);
	}

	std::vector<std::vector<Vertex>> visitedInputCells;

	AddColumnToDKATable(tableDKA, tableNKA, visitedInputCells, cellToAdd, transitions);

	return tableDKA;
}

void CNKAtoDKA::ChangeFinishStates(Table& tableDKA)
{
	for (auto& row : tableDKA)
	{
		for (auto& cell : row)
		{
			for (auto& vertex : cell)
			{
				if (vertex.isFinish)
				{
					for (auto& vertexToChange : cell)
					{
						vertexToChange.isFinish = true;
					}
				}
			}
		}
	}
}

bool CNKAtoDKA::AreEqualTables(const Table& t1, const Table& t2)
{
	if (t1.size() != t2.size()) return false;

	for (size_t rowIndex = 0; rowIndex < t1.size(); rowIndex++)
	{
		if (t1[rowIndex].size() != t2[rowIndex].size()) return false;

		for (size_t columnIndex = 0; columnIndex < t1[rowIndex].size(); columnIndex++)
		{
			if (!AreEqualCells(t1[rowIndex][columnIndex], t2[rowIndex][columnIndex])) return false;
		}
	}

	return true;
}

bool CNKAtoDKA::WereSuchOutputCells(
	const std::vector<std::pair<std::vector<Vertex>, std::vector<std::vector<Vertex>>>>& inputCellOutputCells,
	const std::vector<std::vector<Vertex>>& outputCells
)
{
	for (const auto& pairInputOutput : inputCellOutputCells)
	{
		if (pairInputOutput.second.size() != outputCells.size()) continue;

		bool temp = true;

		for (size_t cellIndex = 0; cellIndex < pairInputOutput.second.size(); cellIndex++)
		{
			if (!AreEqualCells(pairInputOutput.second[cellIndex], outputCells[cellIndex])) temp = false;
		}

		if (temp) return true;
	}

	return false;
}

void CNKAtoDKA::AddOutputCellsToPair(
	std::vector<std::pair<std::vector<Vertex>, std::vector<std::vector<Vertex>>>>& inputCellOutputCells,
	const std::vector<std::vector<Vertex>>& outputCells,
	std::vector<Vertex> inputCell
)
{
	for (auto& pairInputOutput : inputCellOutputCells)
	{
		if (pairInputOutput.second.size() != outputCells.size()) continue;

		bool temp = true;

		for (size_t cellIndex = 0; cellIndex < pairInputOutput.second.size(); cellIndex++)
		{
			if (!AreEqualCells(pairInputOutput.second[cellIndex], outputCells[cellIndex])) temp = false;
		}

		if (temp)
		{
			pairInputOutput.first.insert(pairInputOutput.first.end(), inputCell.begin(), inputCell.end());
			pairInputOutput.first = RemoveDuplicates(pairInputOutput.first);
			for (auto& cell : pairInputOutput.second)
			{
				cell = RemoveDuplicates(cell);
			}

			return;
		}
	}
}

size_t CNKAtoDKA::FindColumnWhichContainsCell(Table table, std::vector<Vertex> cell)
{
	for (size_t columnIndex = 0; columnIndex < table[0].size(); columnIndex++)
	{
		std::vector<Vertex> tableCell = table[0][columnIndex];
		bool next = false;

		for (auto vert : cell)
		{
			if (!IncludeVertexByName(tableCell, vert)) next = true;
		}

		if (next) continue;

		return columnIndex;
	}
}

Table CNKAtoDKA::MinimizeDKATable(const Table& tableDKA)
{
	std::vector<std::pair<std::vector<Vertex>, std::vector<std::vector<Vertex>>>> inputCellOutputCells;
	std::vector<std::vector<Vertex>> outputCells;

	for (size_t columnIndex = 1; columnIndex < tableDKA[0].size(); columnIndex++)
	{
		outputCells.clear();
		for (size_t rowIndex = 1; rowIndex < tableDKA.size(); rowIndex++)
		{
			outputCells.push_back(tableDKA[rowIndex][columnIndex]);
		}

		if (WereSuchOutputCells(inputCellOutputCells, outputCells))
		{
			AddOutputCellsToPair(inputCellOutputCells, outputCells, tableDKA[0][columnIndex]);
		}
		else
		{
			inputCellOutputCells.push_back(std::make_pair(tableDKA[0][columnIndex], outputCells));
		}
	}

	Table minimezedTableDKA(tableDKA.size(), std::vector<std::vector<Vertex>>(inputCellOutputCells.size() + 1, std::vector<Vertex>(0)));

	for (size_t rowIndex = 1; rowIndex < minimezedTableDKA.size(); rowIndex++)
	{
		minimezedTableDKA[rowIndex][0].insert(minimezedTableDKA[rowIndex][0].end(), tableDKA[rowIndex][0].begin(), tableDKA[rowIndex][0].end());
	}

	for (size_t pairIndex = 0; pairIndex < inputCellOutputCells.size(); pairIndex++)
	{
		minimezedTableDKA[0][pairIndex + 1] = inputCellOutputCells[pairIndex].first;

		for (size_t rowIndex = 1; rowIndex < minimezedTableDKA.size(); rowIndex++)
		{
			minimezedTableDKA[rowIndex][pairIndex + 1] = inputCellOutputCells[pairIndex].second[rowIndex - 1];
		}
	}

	for (size_t columnIndex = 1; columnIndex < minimezedTableDKA[0].size(); columnIndex++)
	{
		for (size_t rowIndex = 1; rowIndex < minimezedTableDKA.size(); rowIndex++)
		{
			if (minimezedTableDKA[rowIndex][columnIndex].size() == 0) continue;
			
			size_t column = FindColumnByInputCell(minimezedTableDKA, minimezedTableDKA[rowIndex][columnIndex]);

			if (column == minimezedTableDKA[0].size())
			{				
				column = FindColumnWhichContainsCell(minimezedTableDKA, minimezedTableDKA[rowIndex][columnIndex]);
				
				minimezedTableDKA[rowIndex][columnIndex] = minimezedTableDKA[0][column];
			}
		}
	}

	return minimezedTableDKA;
}

Table CNKAtoDKA::ConvertNKAtoDKA(const Table& tableNKA)
{
	Table tableDKA;
	Table tempTable = tableNKA;
	std::set<std::string> namesOfChangedVertexes;


	Transitions transitions = CreateTransitionsByEmptySymbols(tempTable, namesOfChangedVertexes);

	for (auto& p : transitions)
	{
		if (namesOfChangedVertexes.find(p.first.name) != namesOfChangedVertexes.end()) p.first.isFinish = true;

		for (auto& v : p.second)
		{
			if (namesOfChangedVertexes.find(v.name) != namesOfChangedVertexes.end()) v.isFinish = true;
		}
	}
	
	std::vector<Vertex> cellToAdd;
	cellToAdd.push_back(transitions[0].first);
	tableDKA = CreateDKATable(tempTable, transitions, cellToAdd);
	ChangeFinishStates(tableDKA);

	Table oldTableDKA;
	int i = 1;
	while (!AreEqualTables(tableDKA, oldTableDKA))
	{
		oldTableDKA = tableDKA;
		if (i == 3) return tableDKA;
		tableDKA = MinimizeDKATable(oldTableDKA);
		/*if (i == 1) return tableDKA;*/
		ChangeFinishStates(tableDKA);
		i++;
	}

	return tableDKA;
}