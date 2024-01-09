#pragma once
#include <string>
#include <vector>
#include <iostream>

const char DELIMETER_CSV = ';';
const char FINISH_STATE = 'F';
const std::string EMPTY_SYMBOL = "e";

struct Vertex
{
	std::string name = "";
	bool isFinish = false;
};

using Table = std::vector<std::vector<std::vector<Vertex>>>;
using Transitions = std::vector<std::pair<Vertex, std::vector<Vertex>>>;