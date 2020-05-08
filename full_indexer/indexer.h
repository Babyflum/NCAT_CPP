// Copyright 2018, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

#pragma once

//standard headers
#include <map>
#include <vector>
#include <string>

std::map<std::string, std::map<int, std::vector<int> > > generateIndex(std::map<int, std::vector<std::string> >&);
bool interesting_term(std::string&);