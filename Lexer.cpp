#include "Lexer.h"

std::vector<Token> Lexer::Parse(const std::string &program)
{
	std::vector<Token> tokens {};
	
	Token token {};

	for (char c: program)
	{
		switch (c)
		{
			case '[':
				if (token.type == STRING_LITERAL) 
					{ AppendChar(token); continue; }

				CloseToken(token, tokens);

				token.type = OPEN_BRACKET;
				AppendChar(token);

				CloseToken(token, tokens);
				break;

			case ']':
				if (token.type == STRING_LITERAL) 
					{ AppendChar(token); continue; }

				CloseToken(token, tokens);

				token.type = CLOSE_BRACKET;
				AppendChar(token);

				CloseToken(token, tokens);
				break;

			case '{':
				if (token.type == STRING_LITERAL) 
					{ AppendChar(token); continue; }

				CloseToken(token, tokens);

				token.type = OPEN_CURLY;
				AppendChar(token);

				CloseToken(token, tokens);
				break;

			case '}':
				if (token.type == STRING_LITERAL) 
					{ AppendChar(token); continue; }
				
				CloseToken(token, tokens);

				token.type = CLOSE_CURLY;
				AppendChar(token);

				CloseToken(token, tokens);
				break;

			case ':':
				if (token.type == STRING_LITERAL) 
					{ AppendChar(token); continue; }

				CloseToken(token, tokens);

				token.type = COLON;
				AppendChar(token);

				CloseToken(token, tokens);
				break;

			case '"':
				if (token.type != STRING_LITERAL)
				{
					CloseToken(token, tokens);
					token.type = STRING_LITERAL;
				}
				else if (token.type == STRING_LITERAL)
				{
					CloseToken(token, tokens);
				}
				break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				if (token.type == WHITESPACE)
				{
					token.type = INT_LITERAL;
					AppendChar(token);
				}
				else if (token.type == DOUBLE_LITERAL_CONFUSED)
				{
					token.type = DOUBLE_LITERAL;
					AppendChar(token);
				}
				else 
					{ AppendChar(token); }
				break;

			case '.':
				if (token.type == WHITESPACE)
				{
					token.type = DOUBLE_LITERAL_CONFUSED;
					AppendChar(token);
				}
				else if (token.type == INT_LITERAL)
				{
					token.type = DOUBLE_LITERAL;
					AppendChar(token);
				}
				else if (token.type == STRING_LITERAL)
					{ AppendChar(token); }
				else
				{
					CloseToken(token, tokens);

					token.type = OPERATOR;
					AppendChar(token);

					CloseToken(token, tokens);
				}
				break;

			case '(':
			case ')':
				/* Ignore. */
				break;

			case '=':
			case '+':
			case '-':
			case '*':
			case '<':
			case '>':
			case ';':
			case ',':
				if (token.type != STRING_LITERAL)
				{
					CloseToken(token, tokens);

					token.type = OPERATOR;
					AppendChar(token);

					CloseToken(token, tokens);
				}
				else 
					{ AppendChar(token); }
				break;

			case ' ' :
			case '\t':
				if (token.type == STRING_LITERAL
					|| token.type == COMMENT
					|| token.type == IDENTIFIER)
					{ AppendChar(token); }
				else 
					{ CloseToken(token, tokens); }
				break;

			case '\r':
			case '\n':
				CloseToken(token, tokens);
				++token.lineNumber;
				break;

			case '#':
				if (token.type == STRING_LITERAL)
					{ AppendChar(token); }
				else 
				{
					CloseToken(token, tokens);

					token.type = COMMENT;
					token.text.erase();
				}
				break;
				
			default:
				if (token.type == WHITESPACE
					|| token.type == DOUBLE_LITERAL
					|| token.type == INT_LITERAL)
				{
					CloseToken(token, tokens);

					token.type = IDENTIFIER;
					AppendChar(token);
				}
				else
					{ AppendChar(token); }
				break;
		}
	}

	return tokens;
}

void Lexer::CloseToken(Token &token, std::vector<Token> &tokens)
{
	if (token.type == COMMENT)
		{ std::cout << "[COMMENT: \"" << token.text << "\"]\n"; }
	else if (token.type != WHITESPACE)
		{ tokens.push_back(token); }

	if (token.type == DOUBLE_LITERAL_CONFUSED)
	{
		if (token.text.compare(".") == 0)
			{ token.type = OPERATOR; }
		else 
			{ token.type = DOUBLE_LITERAL; }
	}

	token.type = WHITESPACE;
	token.text.erase();
}

size_t indentation {};

void Token::Info() const
{
	switch (type)
	{
		case OPEN_BRACKET:
		case OPEN_CURLY:
			++indentation;

			for (size_t i{ 0 }; i < (indentation - 1); ++i)
				std::cout << "\t";

			std::cout
				<< "["
				<< TokenTypeStrings[type]
				<< ", \""
				<< text
				<< "\", "
				<< lineNumber
				<< "]\n";

			return;

		case CLOSE_BRACKET:
		case CLOSE_CURLY:
			--indentation;

			for (size_t i {0}; i < indentation; ++i)
				std::cout << "\t";

			std::cout
				<< "["
				<< TokenTypeStrings[type]
				<< ", \""
				<< text
				<< "\", "
				<< lineNumber
				<< "]\n";

			return;
	}

	for (size_t i {0}; i < indentation; ++i)
		std::cout << "\t";

	std::cout 
		<< "["
		<< TokenTypeStrings[type] 
		<< ", \""
		<< text
		<< "\", "
		<< lineNumber
		<< "]\n";
}