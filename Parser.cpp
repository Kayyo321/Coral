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
				// Item found.

				items.push_back(ParseItem(i, token.text, IMMUTABLE));
			}
			else if (tokens[i + 1].type == OPEN_BRACKET)
			{
				// Array found.

				arrays.push_back(ParseArray(i, token.text));
			}
			else if (tokens[i + 1].type == COLON && tokens[i + 2].text == "list")
			{
				i += 2;
				token = tokens[i];

				if (tokens[i + 1].type == OPEN_BRACKET)
				{
					lists.push_back(ParseList(i, token.text));
				}
				else
				{
					throw std::runtime_error(
						std::string("Cannot define non-array: \"")
						+ std::string(token.text)
						+ std::string("\", as a list on line: '")
						+ std::to_string(token.lineNumber)
						+ std::string(", ")
						+ std::to_string(token.charIndex)
						+ std::string("'.")
					);
				}
			}
			else if (tokens[i + 1].type == COLON && tokens[i + 2].type == IDENTIFIER)
			{
				// Mutability expressed.

				i += 2;
				token = tokens[i];

				if (token.text == "mutable ")
				{
					items.push_back(ParseItem(i, tokens[i - 2].text, MUTABLE));
				}
				else if (token.text == "!mutable ")
				{
					items.push_back(ParseItem(i, tokens[i - 2].text, IMMUTABLE));
				}
				else
				{
					throw std::runtime_error(
						std::string("unknown access modifier: \"")
						+ std::string(token.text)
						+ std::string("\", at line: '")
						+ std::to_string(token.lineNumber)
						+ std::string(", ")
						+ std::to_string(token.charIndex)
						+ std::string("'.")
					);
				}
			}
			else
			{
				throw std::runtime_error(
					std::string("Identifier: \"")
					+ std::string(token.text)
					+ std::string("\", without a block at line: '")
					+ std::to_string(token.lineNumber)
					+ std::string(", ")
					+ std::to_string(token.charIndex)
					+ std::string("'.")
				);
			}
		}
	}
}

Item Parser::ParseItem(size_t index, std::string _name, Access _isMutable)
{
	size_t endIndex {}, stage {0};
	
	std::vector<std::string> _keys {};
	std::vector<std::string> _values {};
	std::vector<TokenType> _types {};

	Item item {};
	item.name = _name;
	
	switch (_isMutable)
	{
		case MUTABLE:
			item.isMutable = true;
			break;

		case IMMUTABLE:
			item.isMutable = false;
			break;
	}

	for (size_t i {index + 2}; i < tokens.size(); ++i)
	{
		Token tok {tokens[i]};

		if (tok.type == OPERATOR
			&& tok.text == ",")
			{ stage = 0; }
		else if (tok.type == COLON)
			{ stage = 1; }
		else
		{
			if (tok.type == CLOSE_CURLY)
			{
				endIndex = i;
				
				break;
			}

			if (stage == 0)
			{
				_keys.push_back(tok.text); 
				++item.size;
			}
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

Array Parser::ParseList(size_t index, std::string _name)
{
	Array arr{};
	arr.name = _name;

	size_t endIndex{};

	for (size_t i{ index + 2 }; i < tokens.size(); ++i)
	{
		Token tok{ tokens[i] };

		if (tok.type == IDENTIFIER)
		{
			if (tokens[i + 1].type == OPEN_CURLY)
			{
				arr.items.push_back(ParseItem(i, tok.text, MUTABLE));
			}
			else if (tokens[i + 1].type == OPEN_BRACKET)
			{
				arr.subArrays.push_back(ParseArray(i, tok.text));
			}
			else if (tokens[i + 1].type == COLON && tokens[i + 2].type == IDENTIFIER)
			{
				if (tokens[i + 2].text == "list")
				{
					arr.subLists.push_back(ParseList(i, tok.text));
				}

				throw std::runtime_error(
					std::string("Cannot express mutability: \"")
					+ std::string(token.text)
					+ std::string("\", inside of an array: '")
					+ std::to_string(token.lineNumber)
					+ std::string(", ")
					+ std::to_string(token.charIndex)
					+ std::string("'.")
				);
			}
			else
			{
				throw std::runtime_error(
					std::string("Identifier: \"")
					+ std::string(token.text)
					+ std::string("\", without a block at line: '")
					+ std::to_string(token.lineNumber)
					+ std::string(", ")
					+ std::to_string(token.charIndex)
					+ std::string("'.")
				);
			}

			++arr.size;
		}
		else if (tok.type == CLOSE_BRACKET)
		{
			endIndex = i; break;
		}
	}

	token = tokens[endIndex];

	return arr;
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
		{
			if (tokens[i + 1].type == OPEN_CURLY)
				{ arr.items.push_back(ParseItem(i, tok.text, IMMUTABLE)); }
			else if (tokens[i + 1].type == OPEN_BRACKET)
				{ arr.subArrays.push_back(ParseArray(i, tok.text)); }
			else if (tokens[i + 1].type == COLON && tokens[i + 2].type == IDENTIFIER)
			{
				throw std::runtime_error(
					std::string("Cannot express mutability: \"")
					+ std::string(token.text)
					+ std::string("\", inside of an array: '")
					+ std::to_string(token.lineNumber)
					+ std::string(", ")
					+ std::to_string(token.charIndex)
					+ std::string("'.")
				);
			}
			else
			{
				throw std::runtime_error(
					std::string("Identifier: \"")
					+ std::string(token.text)
					+ std::string("\", without a block at line: '")
					+ std::to_string(token.lineNumber)
					+ std::string(", ")
					+ std::to_string(token.charIndex)
					+ std::string("'.")
				);
			}

			++arr.size; 
		}
		else if (tok.type == CLOSE_BRACKET)
			{ endIndex = i; break; }
	}

	token = tokens[endIndex];

	return arr;
}

void Parser::Output() const
{
	std::vector<Item> ignoreItems {};

	if (arrays.size() > 0)
	{
		std::cout << "\n------- Arrays -------\n\n\n";

		for (const Array &arr: arrays)
		{ 
			std::cout << "Array ("
				<< arr.name 
				<< ", "
				<< arr.size 
				<< "): \n[\n";

			for (const Item &item: arr.items)
			{ 
				ignoreItems.push_back(item);

				std::cout << "\t"
					<< ((item.isMutable) ? "Mutable" : "Immutable")
					<< " Item ("
					<< item.name
					<< ", "
					<< item.size
					<< "): \n\t{\n";

				for (size_t i{ 0 }; i < item.keys.size(); ++i)
				{
					std::cout << "\t\t"
						<< item.keys[i]
						<< ": ( "
						<< TokenTypeStrings[item.types[i]]
						<< " ) \""
						<< item.values[i]
						<< "\"\n";
				}

				std::cout << "\t}\n\n";
			}

			for (const Array &sArr: arr.subArrays)
			{
				std::cout << "\tImmutable Array ( "
					<< sArr.name
					<< ", "
					<< sArr.size
					<< ")\n";
			}

			for (const Array &sList: arr.subLists)
			{
				std::cout << "Mutable List ( "
					<< sList.name
					<< ", "
					<< sList.size
					<< ")\n";
			}

			std::cout << ((arr.size < 1) ? "\t/* Empty Array */\n" : "") << "]\n\n";
		}

		std::cout << "\n";
	}

	if (lists.size() > 0)
	{
		std::cout << "------- lists -------\n\n\n";

		for (const Array &arr: lists)
		{ 
			std::cout << "List ("
				<< arr.name 
				<< ", "
				<< arr.size 
				<< "): \n[\n";

			for (const Item &item: arr.items)
			{ 
				ignoreItems.push_back(item);

				std::cout << "\t"
					<< ((item.isMutable) ? "Mutable" : "Immutable")
					<< " Item ("
					<< item.name
					<< ", "
					<< item.size
					<< "): \n\t{\n";

				for (size_t i{ 0 }; i < item.keys.size(); ++i)
				{
					std::cout << "\t\t"
						<< item.keys[i]
						<< ": ( "
						<< TokenTypeStrings[item.types[i]]
						<< " ) \""
						<< item.values[i]
						<< "\"\n";
				}

				std::cout << "\t}\n\n";
			}

			for (const Array &sArr: arr.subArrays)
			{
				std::cout << "\t"
					<< ((sArr.isMutable) ? "Mutable" : "Immutable")
					<< " Array ( "
					<< sArr.name
					<< ", "
					<< sArr.size
					<< ")\n";
			}

			for (const Array &sList: arr.subLists)
			{
				std::cout << ((sList.isMutable) ? "Mutable" : "Immutable")
					<< " List ( "
					<< sList.name
					<< ", "
					<< sList.size
					<< ")\n";
			}

			std::cout << ((arr.size < 1) ? "\t/* Empty List */\n" : "") << "]\n\n";
		}
	}

	if (items.size() > 0)
	{
		std::cout << "------- Items -------\n\n\n";

		for (const Item &item: items)
		{
			bool ignore {false};

			for (const Item& arritem : ignoreItems)
			{
				if (item.name == arritem.name)
				{
					ignore = true;

					break;
				}
			}

			if (ignore) { continue; }

			std::cout << ((item.isMutable) ? "Mutable" : "Immutable")
				<< " Item (" 
				<< item.name 
				<< ", "
				<< item.size
				<< "): \n{\n";

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

			std::cout << ((item.size < 1) ? "\t/* Empty Item */\n" : "") << "}\n\n";
		}

		std::cout << "\n";
	}
}