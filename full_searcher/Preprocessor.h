// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>


#pragma once


// standard header includes
#include <string>
#include <vector>
#include <cctype>

namespace preprocessor
{
	std::string setOperators(std::string&);
	std::string normalizeWhitespace(std::string&);
	std::vector<std::string> setParentheses(std::vector<std::string>&);
	std::vector<std::string> breakLine(std::string&);
	bool testlegality(std::vector<std::string>&);
	std::vector<std::string> run(std::string&);
}
