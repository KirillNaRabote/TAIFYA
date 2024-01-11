#include <iostream>
#include <fstream>
#include "Lexer.h"

int main()
{
	try
	{
		std::ifstream input("input1.txt");
		std::ofstream output("output1.txt");

		if (!input.is_open() || !output.is_open())
		{
			throw std::exception("Can't open file");
		}

		Lexer lexer(input);

		bool needIterate = false;
		do
		{
			const auto [type, lexeme, lineNum, linePos] = lexer.GetLexeme();
			output << "[" << lineNum << ":" << linePos << "] " << lexeme << " <- " << TokenTypeToString(type) << std::endl;
			needIterate = type != TokenType::END_OF_FILE;
		} while (needIterate);
	}
	catch (const std::exception& ex)
	{
		std::cerr << ex.what() << std::endl;
	}
}