#include "Parser.h"

void Parser::Parse(const std::vector<Token> &_tokens)
{
	tokens = _tokens;
	
	for (size_t i {0}; i < tokens.size(); ++i)
	{
		token = tokens[i];

		if (token.type == IDENTIFIER)
		{
			if (tokens[i + 1].type == OPEN_CURLY)
			{
				items.push_back(ParseItem(i, token.text));
			}
			else if (tokens[i + 1].type == OPEN_BRACKET)
			{
				arrays.push_back(ParseArray(i, token.text));
			}
			else
			{
				throw std::runtime_error(
					std::string("Identifier without a block at line: '")
					+ std::to_string(token.lineNumber)
					+ std::string(", ")
					+ std::to_string(token.charIndex)
					+ std::string("'.")
				);
			}
		}
	}
}

Item Parser::ParseItem(size_t index, std::string _name)
{
	size_t endIndex {}, stage {0};
	
	std::vector<std::string> _keys {};
	std::vector<std::string> _values {};
	std::vector<TokenType> _types {};

	Item item {};
	item.name = _name;

	for (size_t i {index + 2}; i < tokens.size(); ++i)
	{
		Token tok {tokens[i]};

		if (tok.type == OPERATOR
			&& tok.text == ",")
		{
			stage = 0;
		}
		else if (tok.type == COLON)
			{ stage = 1; }
		else
		{
			if (tok.type == CLOSE_CURLY)
				{ endIndex = i; break; }

			if (stage == 0)
				{ _keys.push_back(tok.text); }
			else if (stage == 1)
			{
				_values.push_back(tok.text);
				_types.push_back(tok.type);
			}
		}
	}

	token = tokens[endIndex];

	item.keys = _keys;
	item.values = _values;
	item.types = _types;

	return item;
}

Array Parser::ParseArray(size_t index, std::string _name)
{
	Array arr {};
	arr.name = _name;

	size_t endIndex {};

	for (size_t i {index + 2}; i < tokens.size(); ++i)
	{
		Token tok {tokens[i]};

		if (tok.type == IDENTIFIER)
			{ arr.items.push_back(ParseItem(i, tok.text)); ++arr.size; }
		else if (tok.type == CLOSE_BRACKET)
			{ endIndex = i; break; }
	}

	token = tokens[endIndex];

	return arr;
}

void Parser::Output() const
{
	if (arrays.size() > 0)
	{
		for (const Array &arr: arrays)
		{ 
			std::cout << "Array ("
				<< arr.name 
				<< ", "
				<< arr.size 
				<< "): \n[\n";

			for (const Item &item: arr.items)
			{
				std::cout << "\tItem ( " << item.name << ")\n";
			}

			std::cout << "]\n\n";
		}

		std::cout << "\n";
	}		

	if (items.size() > 0)
	{
		for (Item item: items)
		{
			std::cout << "Item ( " << item.name << "): \n{\n";

			for (size_t i {0}; i < item.keys.size(); ++i)
			{
				std::cout << "\t"
					<< item.keys[i]
					<< ": ( "
					<< TokenTypeStrings[item.types[i]]
					<< " ) \""
					<< item.values[i]
					<< "\"\n";
			}

			std::cout << "}\n\n";
		}

		std::cout << "\n";
	}
}