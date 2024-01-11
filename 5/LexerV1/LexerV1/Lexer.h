#pragma once

#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <optional>
#include <istream>
#include <string>
#include <regex>
#include "TokenType.h"

namespace
{
	const std::unordered_set<char> IGNORED_CHARS = { ' ', '\t', '\n' };
	const std::unordered_set<char> SEPARATORS = { ' ', '\t', '\n', ';', ',', '{', '}', '(', ')', '[', ']', '=', '<', '>', '!', '/', '*', '+', '-', '"' };

	bool IsNumber(const std::string& lexeme)
	{
		return std::regex_match(lexeme, std::regex("[-+]?\\d+"))
			|| std::regex_match(lexeme, std::regex("[-+]?0b[01]+"))
			|| std::regex_match(lexeme, std::regex("[-+]?0x[0-9A-F]+"))
			|| std::regex_match(lexeme, std::regex("[-+]?\\d+\\.\\d+"))
			|| std::regex_match(lexeme, std::regex("[-+]?\\.\\d+"))
			|| std::regex_match(lexeme, std::regex("[-+]?\\d+\\."))
			|| std::regex_match(lexeme, std::regex("[-+]?\\d+[eE][-+]?\\d+"));
	}

	bool IsIdentifier(const std::string& lexeme)
	{
		return std::regex_match(lexeme, std::regex("[a-zA-Z_][a-zA-Z0-9_]*"));
	}

	TokenType ClassifyLexeme(const std::string& lexeme)
	{
		if (lexeme == "void") return TokenType::KEYWORD;
		if (lexeme == "string") return TokenType::KEYWORD;
		if (lexeme == "double") return TokenType::KEYWORD;
		if (lexeme == "int") return TokenType::KEYWORD;
		if (lexeme == "bool") return TokenType::KEYWORD;
		if (lexeme == "while") return TokenType::KEYWORD;
		if (lexeme == "break") return TokenType::KEYWORD;
		if (lexeme == "continue") return TokenType::KEYWORD;
		if (lexeme == "if") return TokenType::KEYWORD;
		if (lexeme == "else") return TokenType::KEYWORD;
		if (lexeme == "return") return TokenType::KEYWORD;

		if (lexeme == "+") return TokenType::MATH_OPERATION;
		if (lexeme == "-") return TokenType::MATH_OPERATION;
		if (lexeme == "*") return TokenType::MATH_OPERATION;
		if (lexeme == "/") return TokenType::MATH_OPERATION;
		if (lexeme == "=") return TokenType::MATH_OPERATION;

		if (lexeme == "==") return TokenType::LOGICAL_OPERATOR;
		if (lexeme == "&&") return TokenType::LOGICAL_OPERATOR;
		if (lexeme == "||") return TokenType::LOGICAL_OPERATOR;
		if (lexeme == "<") return TokenType::LOGICAL_OPERATOR;
		if (lexeme == ">") return TokenType::LOGICAL_OPERATOR;
		if (lexeme == "!") return TokenType::LOGICAL_OPERATOR;

		if (lexeme == "(") return TokenType::SEPARATOR;
		if (lexeme == ")") return TokenType::SEPARATOR;
		if (lexeme == "{") return TokenType::SEPARATOR;
		if (lexeme == "}") return TokenType::SEPARATOR;
		if (lexeme == "[") return TokenType::SEPARATOR;
		if (lexeme == "]") return TokenType::SEPARATOR;
		if (lexeme == ";") return TokenType::SEPARATOR;
		if (lexeme == ",") return TokenType::SEPARATOR;
		if ((lexeme.size() > 1) && (lexeme.front() == '"') && (lexeme.back() == '"')) return TokenType::STRING;
		if ((lexeme == "true") || (lexeme == "false")) return TokenType::BOOLEAN;
		if (IsNumber(lexeme)) return TokenType::NUMBER;
		if (IsIdentifier(lexeme)) return TokenType::IDENTIFIER;
		return TokenType::ERROR;
	}

	std::string TokenTypeToString(TokenType type)
	{
		switch (type)
		{
		case TokenType::KEYWORD:
			return "KEYWORD";

		case TokenType::IDENTIFIER:
			return "IDENTIFIER";

		case TokenType::SEPARATOR:
			return "SEPARATOR";

		case TokenType::MATH_OPERATION:
			return "MATH_OPERATION";

		case TokenType::LOGICAL_OPERATOR:
			return "LOGICAL_OPERATOR";

		case TokenType::STRING:
			return "STRING";
		case TokenType::NUMBER:
			return "NUMBER";
		case TokenType::BOOLEAN:
			return "BOOLEAN";

		case TokenType::END_OF_FILE:
			return "END_OF_FILE";
		case TokenType::ERROR:
		default:
			return "ERROR";
		}
	}

	class EndOfFileException
		:public std::exception
	{
	public:
		explicit EndOfFileException(bool isControlled = true)
			:std::exception()
			, m_isControlled(isControlled)
		{}

		bool isControlled() const
		{
			return m_isControlled;
		}

	private:
		bool m_isControlled = false;
	};

};

struct Lexeme
{
	TokenType type;
	std::string lexeme;
	size_t lineNum;
	size_t linePos;
};


class Lexer
{
public:
	Lexer(std::istream& strm);

	Lexeme GetLexeme();

private:
	std::string GetLexemeImpl();

	void SkipIgnored();

	std::string ProcessString();

	std::string ProcessEqual();

	std::string ProcessComment();
	std::string ProcessMultiLineComment();

	void UpdateCurrentLine(char ch);

	std::istream& GetNextChar(char& ch);

	void PutCharBack();

	std::istream& m_strm;
	size_t m_currentLine = 1;
	size_t m_currentPos = 0;
};