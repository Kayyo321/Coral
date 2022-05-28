#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>

#include "Lexer.h"

class Item
{
public:
	std::string name;

	std::vector<std::string> keys;
	std::vector<std::string> values;
	std::vector<enum TokenType> types;

	size_t size;

	bool isMutable {false};
	bool isEmpty {false};
};

class Array
{
public:
	std::string name;
	
	std::vector<Item> items;
	std::vector<Array> subArrays;

	size_t size;

	bool isMutable {false};
};

enum Access
{
	MUTABLE,
	IMMUTABLE
};

class Parser
{
public:
	void Parse(const std::vector<Token>&);

	void Output() const;

	std::vector<Item> GetItems() { return items; }
	std::vector<Array> GetArrays() { return arrays; }

private:
	Item ParseItem(size_t, std::string, Access);
	Array ParseArray(size_t, std::string);

	std::vector<Array> arrays;
	std::vector<Item> items;

	std::vector<Token> tokens;
	Token token;
};