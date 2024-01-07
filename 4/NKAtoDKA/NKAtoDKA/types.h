#pragma once
#include <string>
#include <vector>

struct Vertex
{
	std::string name = "";
	bool isFinish = false;
};

using Table = std::vector<std::vector<std::vector<Vertex>>>; // ¬нутренний вектор нужен, так как в €чейке таблицы может быть несколько вершин