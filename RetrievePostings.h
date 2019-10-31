// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

#pragma once

// standard headers
#include <vector>
#include <string>
#include <map>


// basic retrieval function for InvertedIndex
std::map<int, std::vector<int> > retrieve(std::string&, std::map<std::string, std::map<int, std::vector<int> > >&);