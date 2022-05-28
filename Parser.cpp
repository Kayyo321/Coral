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
					+ std::string("'.")
				);
			}
		}
	}
}

Item Parser::ParseItem(size_t index, std::string _name)
{
	size_t endIndex {}, stage {0}, stage1 {0};
	std::string lhs {}, rhs {},
				lhs1 {}, rhs1 {};

	Item item{};

	for (size_t i {index + 2}; i < tokens.size(); ++i)
	{
		Token tok {tokens[i]};

		if (tok.type == OPERATOR
			&& tok.text == ",")
		{
			stage = 1;
			stage1 = 0;
		}
		else if (tok.type == COLON)
			{ stage1 = 1; }
		else
		{
			if (tok.type == CLOSE_CURLY)
				{ endIndex = i; break; }

			switch (stage)
			{
				case 0:
					if (stage1 == 0)
						{ lhs += tok.text; }
					else if (stage1 == 1)
						{ rhs += tok.text; item.type1 = tok.type; }
					break;

				case 1:
					if (stage1 == 0)
						{ lhs1 += tok.text; }
					else if (stage1 == 1)
						{ rhs1 += tok.text; item.type2 = tok.type; }
					break;
			}
		}
	}

	token = tokens[endIndex];

	item.name = _name;

	item.title1 = lhs;
	item.value1 = rhs;

	item.title2 = lhs1;
	item.value2 = rhs1;

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
				<< "): \n{\n";

			for (const Item &item: arr.items)
			{
				std::cout << "\tItem ( " << item.name << ")\n";
			}

			std::cout << "}\n\n";
		}

		std::cout << "\n";
	}		

	if (items.size() > 0)
	{
		for (Item item: items)
		{
			std::cout << "Item ( " << item.name << "): \n[\n\t\""
				<< item.title1 << "\": \""
				<< "(" << TokenTypeStrings[item.type1] << ") " << item.value1 
				<< "\", \n\t\""
				<< item.title2 << "\": \""
				<< "(" << TokenTypeStrings[item.type2] << ") " << item.value2
				<< "\"\n]\n\n";
		}

		std::cout << "\n";
	}
}