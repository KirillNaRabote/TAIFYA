#include "Lexer.h"

Lexer::Lexer(std::istream& strm)
	:m_strm(strm)
{
	m_strm >> std::noskipws;
}

Lexeme Lexer::GetLexeme()
{
	std::string lexeme;
	do
	{
		try
		{
			lexeme = GetLexemeImpl();
		}
		catch (const EndOfFileException& ex)
		{
			return { ex.isControlled() ? TokenType::END_OF_FILE : TokenType::ERROR, "", m_currentLine, m_currentPos };
		}
	} while (lexeme.empty());
	return { ClassifyLexeme(lexeme), lexeme, m_currentLine, m_currentPos };
}

std::string Lexer::GetLexemeImpl()
{
	{
		SkipIgnored();
		if (m_strm.eof())
		{
			throw EndOfFileException();
		}

		char ch;
		std::string lexeme;
		auto wasIterated = false;
		while (!m_strm.eof() && GetNextChar(ch))
		{
			wasIterated = true;

			if (SEPARATORS.count(ch))
			{
				if (!lexeme.empty() || IGNORED_CHARS.count(ch))
				{
					break;
				}
				UpdateCurrentLine(ch);

				if (ch == '=')
				{
					return ProcessEqual();
				}
				if ((ch == '+') || (ch == '-') || (ch == 'e') || (ch == 'E'))
				{
					const auto nextCh = m_strm.peek();
					if (isdigit(nextCh) || (nextCh == '.'))
					{
						lexeme += ch;
						continue;
					}
				}
				if (ch == '/')
				{
					const auto nextCh = m_strm.peek();
					if (nextCh == '/')
					{
						return ProcessComment();
					}
					else if (nextCh == '*')
					{
						return ProcessMultiLineComment();
					}
				}
				if (ch == '"')
				{
					return ProcessString();
				}
				return std::string(1, ch);
			}
			lexeme += ch;
		}
		if (wasIterated)
		{
			PutCharBack();
		}
		return lexeme;
	}
}

std::string Lexer::ProcessComment()
{
	char ch;
	while (!m_strm.eof() && GetNextChar(ch) && (ch != '\n'))
	{
		UpdateCurrentLine(ch);
	}
	if (m_strm.eof())
	{
		throw EndOfFileException(false);
	}
	return "";
}

std::string Lexer::ProcessMultiLineComment()
{
	char ch;

	bool cont = true;
	while (!m_strm.eof() && GetNextChar(ch) && cont)
	{
		const auto nextCh = m_strm.peek();

		if (ch == '*' && nextCh == '/')
		{
			GetNextChar(ch);
			cont = false;
		}

		UpdateCurrentLine(ch);
	}
	if (m_strm.eof())
	{
		throw EndOfFileException(false);
	}

	return "";
}

void Lexer::SkipIgnored()
{
	char ch;
	while (!m_strm.eof() && GetNextChar(ch))
	{
		UpdateCurrentLine(ch);
		if (!IGNORED_CHARS.count(ch))
		{
			PutCharBack();
			break;
		}
	}
}

std::string Lexer::ProcessString()
{
	std::string lexeme;

	char ch;
	while (!m_strm.eof() && GetNextChar(ch) && (ch != '"'))
	{
		UpdateCurrentLine(ch);
		lexeme += ch;
	}
	if (m_strm.eof())
	{
		throw EndOfFileException(false);
	}
	return '"' + lexeme + '"';
}

std::string Lexer::ProcessEqual()
{
	if (!m_strm.eof())
	{
		char ch;
		if (GetNextChar(ch) && (ch == '='))
		{
			return "==";
		}
		else
		{
			PutCharBack();
			return "=";
		}
	}
	throw EndOfFileException();
}

void Lexer::UpdateCurrentLine(char ch)
{
	if (ch == '\n')
	{
		++m_currentLine;
		m_currentPos = 0;
	}
}

std::istream& Lexer::GetNextChar(char& ch)
{
	m_strm >> ch;
	++m_currentPos;
	return m_strm;
}

void Lexer::PutCharBack()
{
	m_strm.unget();
	if (m_currentPos != 0)
	{
		--m_currentPos;
	}
}
