// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>


std::vector<std::string> lineToVec(std::string&);
std::vector<std::vector<std::string> > readFile(std::ifstream&);
bool isSeparator(const char);
bool isAcronym(std::string::const_iterator, const std::string& str);
std::vector<std::string> split(const std::string&);
std::map<int, std::vector<std::string> > genMap(std::vector<std::vector<std::string> >&, std::map<int, std::string>&);
std::map<int, std::vector<std::string> > RunMain(std::string);