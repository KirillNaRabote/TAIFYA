#pragma once

#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <algorithm>

#include "types.h"

class CFileHandler
{
public:
	static Table ReadTable(std::ifstream& input);
	static void PrintTable(std::ofstream& output, const Table& table);
private:
	static void AddLineInTable(Table& table, const std::vector<std::vector<Vertex>>& tableLine, int lineCounter);
};