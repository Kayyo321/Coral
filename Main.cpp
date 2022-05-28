#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"

std::vector<Item> programItems {};
std::vector<Array> programArrays {};

std::string ReadFile(const std::string &path)
{
	std::ifstream file {path};

	if (!file.is_open())
	{
		std::cerr << "Could not open the file: \""
			<< path << "\".\n";

		exit(1);
	}

	return std::string(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
}

void Output()
{
	std::vector<Item> ignoreItems{};

	if (programArrays.size() > 0)
	{
		std::cout << "\n------- Arrays -------\n\n\n";

		for (const Array& arr : programArrays)
		{
			std::cout << "Array ("
				<< arr.name
				<< ", "
				<< arr.size
				<< "): \n[\n";

			for (const Item &item : arr.items)
			{
				ignoreItems.push_back(item);

				std::cout << "\t"
					<< ((item.isMutable) ? "Mutable" : "Un-Mutable")
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

			for (const Array& sArr : arr.subArrays)
			{
				std::cout << "\t"
					<< ((sArr.isMutable ? "Mutable" : "Un-Mutable"))
					<< " Array ( "
					<< sArr.name
					<< ")\n";
			}

			std::cout << "]\n\n";
		}

		std::cout << "\n";
	}

	if (programItems.size() > 0)
	{
		std::cout << "------- Items -------\n\n\n";

		for (const Item &item : programItems)
		{
			bool ignore{ false };

			for (const Item &arritem : ignoreItems)
			{
				if (item.name == arritem.name)
				{
					ignore = true;

					break;
				}
			}

			if (ignore) { continue; }

			std::cout << ((item.isMutable) ? "Mutable" : "Un-Mutable")
				<< " Item ("
				<< item.name
				<< ", "
				<< item.size
				<< "): \n{\n";

			for (size_t i{ 0 }; i < item.keys.size(); ++i)
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

int main()
{
	std::cout << "path> ";

	std::string path {};
	std::cin >> path;

	std::string file {ReadFile(path)};

	std::cout << "\n\n--== File ==--\n\n"
		<< file;

	std::cout << "\n\n--== Lexer ==--\n\n";

	Lexer lexer;
	std::vector<Token> tokens {lexer.Parse(file)};

	for (const Token &token: tokens)
		{ token.Info(); }

	std::cout << "\n\n--== Parser ==--\n\n";

	try
	{
		Parser parser;
		parser.Parse(tokens);

		parser.Output();

		programItems = parser.GetItems();
		programArrays = parser.GetArrays();
	}
	catch (std::runtime_error e)
	{
		std::cerr << "Error occurred: \"" << e.what() << "\"\n";

		return 1;
	}

	return 0;
}