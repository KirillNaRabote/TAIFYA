#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <regex>

#include "types.h"

const char DELIMETER = ';';

Table ReadTable(std::ifstream& input)
{
	Table table;
	std::string finishLine;
	std::string line;
	std::map<std::string, bool> AreFinishStates;
	std::vector<int> indexesOfFinishStates = {};
	
	std::regex pattern("[^,s]+");

	getline(input, finishLine);
	getline(input, line);
	
	for (size_t indexOfFinishLine = 0; indexOfFinishLine < finishLine.size(); indexOfFinishLine++)
	{

	}

	return table;
}