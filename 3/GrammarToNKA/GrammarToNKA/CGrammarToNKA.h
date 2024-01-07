#pragma once

#include "types.h"

class CGrammarToNKA
{
public:
	static TableNKA GrammarToNKA(
		const Grammar& grammar,
		const VecOfStr& nonTerminalSymbols,
		const VecOfStr& terminalSymbols,
		const std::string& type
	);

private:
	static void RightGrammarToNKA(TableNKA& table, const Grammar& grammar);
	static void LeftGrammarToNKA(TableNKA& table, const Grammar& grammar);

	static size_t FindRow(const TableNKA& table, const std::string& terminalSymbol);
	static size_t FindColumn(const TableNKA& table, const std::string& terminalSymbol);
};