#include "CFileHandler.h"

void CFileHandler::AddLineInTable(Table& table, const std::vector<std::vector<Vertex>>& tableLine, int lineCounter)
{
	std::regex cellPattern("[^,]+");
	size_t tableRowSize;
	(lineCounter == 1)
		? tableRowSize = tableLine.size()
		: tableRowSize = table[0].size();

	std::vector<std::vector<Vertex>> newTableLine(tableRowSize);

	for (size_t cellIndex = 0; cellIndex < tableLine.size(); cellIndex++)
	{
		std::vector<Vertex> newCell;

		for (size_t stateIndex = 0; stateIndex < tableLine[cellIndex].size(); stateIndex++)
		{
			std::string cellName = tableLine[cellIndex][stateIndex].name;
			std::regex_iterator<std::string::iterator> iterator(cellName.begin(), cellName.end(), cellPattern);
			std::regex_iterator<std::string::iterator> end;

			while (iterator != end) {
				Vertex newVertex;
				newVertex.name = iterator->str();

				newCell.push_back(newVertex);

				++iterator;
			}
		}

		newTableLine[cellIndex] = newCell;
	}

	table.push_back(newTableLine);
}

Table CFileHandler::ReadTable(std::ifstream& input)
{
	Table table;
	std::string finishLine;
	std::string line;
	std::map<std::string, bool> AreFinishStates;
	std::vector<int> indexesOfFinishStates = {};

	getline(input, finishLine);

	size_t pos = 0;
	for (size_t indexOfFinishLine = 0; indexOfFinishLine < finishLine.size(); indexOfFinishLine++)
	{
		if (finishLine[indexOfFinishLine] == DELIMETER_CSV)
		{
			pos++;
		}
		else if (finishLine[indexOfFinishLine] == FINISH_STATE)
		{
			indexesOfFinishStates.push_back(pos);
		}
		else
		{
			throw std::invalid_argument("Unknown symbol in 1 line");
		}
	}

	int lineCounter = 1;

	while (getline(input, line))
	{
		line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());

		int cellCounter = 0;
		std::string vert = "";
		std::vector<std::vector<Vertex>> tableLine(pos + 1, std::vector<Vertex>(1));

		for (size_t index = 0; index < line.size(); index++)
		{
			if (line[index] == DELIMETER_CSV || index == line.size() - 1)
			{
				if (index == line.size() - 1 && vert != "" && line[index] != DELIMETER_CSV)
				{
					vert += line[index];
				}
				if (vert != "")
				{
					std::vector<Vertex> cell;
					Vertex ver;

					ver.name = vert;
					cell.push_back(ver);
					tableLine[cellCounter] = cell;
				}
				cellCounter++;
				vert = "";
				continue;
			}

			vert += line[index];
		}

		AddLineInTable(table, tableLine, lineCounter);

		lineCounter++;
	}

	for (const auto posOfFinish : indexesOfFinishStates)
	{
		table[0][posOfFinish][0].isFinish = true;
		AreFinishStates[table[0][posOfFinish][0].name] = true;
	}

	for (size_t rowIndex = 0; rowIndex < table.size(); rowIndex++)
	{
		for (size_t columnIndex = 0; columnIndex < table[rowIndex].size(); columnIndex++)
		{
			for (size_t stateIndex = 0; stateIndex < table[rowIndex][columnIndex].size(); stateIndex++)
			{
				if (AreFinishStates[table[rowIndex][columnIndex][stateIndex].name] == true)
				{
					table[rowIndex][columnIndex][stateIndex].isFinish = true;
				}
			}
		}
	}

	return table;
}

void CFileHandler::PrintTable(std::ofstream& output, const Table& table)
{
	std::string firstString = ";";
	for (size_t columnIndex = 1; columnIndex < table[0].size(); columnIndex++)
	{
		if (table[0][columnIndex][0].isFinish)
		{
			firstString += FINISH_STATE;
		}
		firstString += ";";
	}

	output << firstString << "\n";

	for (size_t rowIndex = 0; rowIndex < table.size(); rowIndex++)
	{
		for (size_t columnIndex = 0; columnIndex < table[rowIndex].size(); columnIndex++)
		{
			for (size_t stateIndex = 0; stateIndex < table[rowIndex][columnIndex].size(); stateIndex++)
			{
				output << table[rowIndex][columnIndex][stateIndex].name;
				
				if (stateIndex != table[rowIndex][columnIndex].size() - 1) output << "/";
			}
			output << DELIMETER_CSV;
		}
		output << "\n";
	}
}