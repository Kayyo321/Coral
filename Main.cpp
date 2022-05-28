#include <iostream>
#include <fstream>
#include <sstream>

#include "Lexer.h"
#include "Parser.h"

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

	std::cout << "\n\n--== Parser Output ==--\n\n";

	Parser parser;
	parser.Parse(tokens);

	parser.Output();

	return 0;
}