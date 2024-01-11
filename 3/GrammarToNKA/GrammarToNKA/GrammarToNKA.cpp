#include <iostream>
#include <regex>

#include "InputOutputHandler.h"
#include "FileHandler.h"
#include "CGrammarToNKA.h"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		CheckInputParameters(argc);

		string inputFileName = argv[1];
		ifstream inputFile(inputFileName);

		string outputFileName = argv[2];
		ofstream outputFile(outputFileName);

		CheckFile(inputFile, inputFileName);
		CheckFile(outputFile, outputFileName);

		Grammar grammar;
		VecOfStr nonTerminalSymbols;
		VecOfStr terminalSymbols;
		string type;

		ReadGrammar(inputFile, grammar, nonTerminalSymbols, terminalSymbols, type);

		TableNKA table = CGrammarToNKA::GrammarToNKA(grammar, nonTerminalSymbols, terminalSymbols, type);
		
		PrintTableNKA(outputFile, table);
	}
	catch (exception e)
	{
		cout << e.what();
		return 1;
	}

	return 0;
}