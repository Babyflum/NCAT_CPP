// Copyright 2020, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

#pragma once

// standard headers
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>

bool wordIsValid(std::string&);
std::string permuterm_rotate(std::string&);
std::map<std::string, std::string> generatePI(std::ifstream&);
void runPI();