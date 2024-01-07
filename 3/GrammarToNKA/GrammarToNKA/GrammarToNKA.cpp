#include <iostream>
#include <regex>

#include "InputOutputHandler.h"
#include "FileHandler.h"
#include "CGrammarToNKA.h"

using namespace std;

//void PrintGrammar(const Grammar& grammar)
//{
//	for (const auto& rule : grammar)
//	{
//		cout << rule.first << "=";
//
//		for (const auto& signalAndState : rule.second)
//		{
//			cout << signalAndState.first << "-" << signalAndState.second << "|";
//		}
//		cout << endl;
//	}
//}
//
//template<typename T>
//void PrintVector(const vector<T>& vec)
//{
//	for (const auto& element : vec)
//	{
//		cout << element << " ";
//	}
//	cout << endl;
//}

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