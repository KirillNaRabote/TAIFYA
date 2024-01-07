#pragma once
#include <string>
#include <vector>

struct Vertex
{
	std::string name = "";
	bool isFinish = false;
};

using Table = std::vector<std::vector<std::vector<Vertex>>>; // ���������� ������ �����, ��� ��� � ������ ������� ����� ���� ��������� ������