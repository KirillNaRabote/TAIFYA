#include "CGrammarToNKA.h"

size_t CGrammarToNKA::FindRow(const TableNKA& table, const std::string& terminalSymbol)
{
	for (size_t row = 1; row < table.size(); row++)
	{
		if (table[row][0] == terminalSymbol)
		{
			return row;
		}
	}
}

size_t CGrammarToNKA::FindColumn(const TableNKA& table, const std::string& NonTerminalSymbol)
{
	for (size_t column = 1; column < table[0].size(); column++)
	{
		if (table[0][column] == NonTerminalSymbol)
		{
			return column;
		}
	}
}

void CGrammarToNKA::RightGrammarToNKA(TableNKA& table, const Grammar& grammar)
{
	size_t row, column;
	for (const auto& rule : grammar)
	{
		column = FindColumn(table, rule.first);

		for (const auto& SignalAndOutput : rule.second)
		{
			row = FindRow(table, SignalAndOutput.first);

			(table[row][column] == "-")
				? table[row][column] = SignalAndOutput.second
				: table[row][column] += "/" + SignalAndOutput.second;
		}
	}
}

void CGrammarToNKA::LeftGrammarToNKA(TableNKA& table, const Grammar& grammar)
{
	size_t row, column;
	for (int grammarIndex = grammar.size() - 1; grammarIndex >= 0; grammarIndex--)
	{
		const RulesForOneState rule = grammar[grammarIndex];
		for (int ruleIndex = rule.second.size() - 1; ruleIndex >= 0; ruleIndex--)
		{
			std::pair<std::string, std::string> SignalAndOutput = rule.second[ruleIndex];
			row = FindRow(table, SignalAndOutput.first);
			column = FindColumn(table, SignalAndOutput.second);

			(table[row][column] == "-")
				? table[row][column] = rule.first
				: table[row][column] += "/" + rule.first;
		}
	}
}

TableNKA CGrammarToNKA::GrammarToNKA(
	const Grammar& grammar,
	const VecOfStr& nonTerminalSymbols,
	const VecOfStr& terminalSymbols,
	const std::string& type
)
{
	TableNKA table(terminalSymbols.size() + 1, VecOfStr(nonTerminalSymbols.size() + 1, "-"));
	table[0][0] = "";
	size_t row, column;

	for (row = 1; row < table.size(); row++)
	{
		table[row][0] = terminalSymbols[row - 1];
	}

	for (column = 1; column < table[0].size(); column++)
	{
		table[0][column] = nonTerminalSymbols[column - 1];
	}

	(type == RIGHT_GRAMMAR_STR) ? RightGrammarToNKA(table, grammar) : LeftGrammarToNKA(table, grammar);

	return table;
}