#pragma once
#include <map>

//class CTokenTypeInitializer
//{
//public:
//	static std::map<std::string, std::string> Initialize()
//	{
//		std::map<std::string, std::string> tokenTypesList;
//
//		tokenTypesList.emplace("void", "KEYWORD");
//		tokenTypesList.emplace("string", "KEYWORD");
//		tokenTypesList.emplace("double", "KEYWORD");
//		tokenTypesList.emplace("int", "KEYWORD");
//		tokenTypesList.emplace("bool", "KEYWORD");
//		tokenTypesList.emplace("while", "KEYWORD");
//		tokenTypesList.emplace("break", "KEYWORD");
//		tokenTypesList.emplace("continue", "KEYWORD");
//		tokenTypesList.emplace("if", "KEYWORD");
//		tokenTypesList.emplace("else", "KEYWORD");
//		tokenTypesList.emplace("return", "KEYWORD");
//
//		tokenTypesList.emplace("+", "MATH_OPERATION");
//		tokenTypesList.emplace("-", "MATH_OPERATION");
//		tokenTypesList.emplace("*", "MATH_OPERATION");
//		tokenTypesList.emplace("/", "MATH_OPERATION");
//		tokenTypesList.emplace("=", "MATH_OPERATION");
//
//		tokenTypesList.emplace("==", "LOGICAL_OPERATOR");
//		tokenTypesList.emplace("&&", "LOGICAL_OPERATOR");
//		tokenTypesList.emplace("||", "LOGICAL_OPERATOR");
//		tokenTypesList.emplace("<", "LOGICAL_OPERATOR");
//		tokenTypesList.emplace(">", "LOGICAL_OPERATOR");
//		tokenTypesList.emplace("!", "LOGICAL_OPERATOR");
//
//		tokenTypesList.emplace("(", "SEPARATOR");
//		tokenTypesList.emplace(")", "SEPARATOR");
//		tokenTypesList.emplace("{", "SEPARATOR");
//		tokenTypesList.emplace("}", "SEPARATOR");
//		tokenTypesList.emplace("[", "SEPARATOR");
//		tokenTypesList.emplace("]", "SEPARATOR");
//		tokenTypesList.emplace(";", "SEPARATOR");
//		tokenTypesList.emplace(",", "SEPARATOR");
//
//		return tokenTypesList;
//	}
//};
enum class TokenType
{
	IDENTIFIER,

	KEYWORD,

	SEPARATOR,

	MATH_OPERATION,

	LOGICAL_OPERATOR,

	STRING,
	NUMBER,
	BOOLEAN,

	END_OF_FILE,
	ERROR,
};