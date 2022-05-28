#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

#include "Lexer.h"

class Item
{
public:
	std::string name;

	std::vector<std::string> keys;
	std::vector<std::string> values;
	std::vector<TokenType> types;
};

class Array
{
public:
	std::string name;
	
	std::vector<Item> items;

	size_t size;
};

class Parser
{
public:
	void Parse(const std::vector<Token>&);

	void Output() const;

private:
	Item ParseItem(size_t, std::string);
	Array ParseArray(size_t, std::string);

	std::vector<Token> tokens;
	Token token;

	std::vector<Array> arrays;
	std::vector<Item> items;
};