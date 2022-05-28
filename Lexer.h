#pragma once

#define AppendChar(x) x.text.append(1, c)

#include <string>
#include <iostream>
#include <vector>

enum TokenType
{
	WHITESPACE,
	IDENTIFIER,
	OPEN_BRACKET,
	CLOSE_BRACKET,
	OPEN_CURLY,
	CLOSE_CURLY,
	COLON,
	STRING_LITERAL,
	INT_LITERAL,
	DOUBLE_LITERAL,
	DOUBLE_LITERAL_CONFUSED,
	OPERATOR,
	COMMENT
};

static const std::string TokenTypeStrings[] =
{
	"WHITESPACE",
	"IDENTIFIER",
	"OPEN_BRACKET",
	"CLOSE_BRACKET",
	"OPEN_CURLY",
	"CLOSE_CURLY",
	"COLON",
	"STRING_LITERAL",
	"INT_LITERAL",
	"DOUBLE_LITERAL",
	"DOUBLE_LITERAL_CONFUSED",
	"OPERATOR",
	"COMMENT"
};

class Token
{
public:
	TokenType type {};
	std::string text;
	size_t lineNumber {1};

	void Info() const;
};

class Lexer
{
public:
	std::vector<Token> Parse(const std::string&);

private:
	void CloseToken(Token&, std::vector<Token>&);
};