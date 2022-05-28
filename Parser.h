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

	std::string title1;
	std::string value1;
	
	std::string title2;
	std::string value2;

	TokenType type1;
	TokenType type2;
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