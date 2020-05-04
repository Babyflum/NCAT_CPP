// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

#pragma once


// standard headers
#include <vector>
#include <map>


// ____________________________________________________________
std::vector<int> mergePos(std::vector<int>&, std::vector<int>&);
// ____________________________________________________________
std::map<int, std::vector<int> > intersect(std::map<int, std::vector<int> >&, std::map<int, std::vector<int> >&);
// ____________________________________________________________
std::map<int, std::vector<int> > unionize(std::map<int, std::vector<int> >&, std::map<int, std::vector<int> >&);
// ____________________________________________________________
std::map<int, std::vector<int> > complement(std::map<int, std::vector<int> >&, std::map<int, std::vector<int> >&);
// ____________________________________________________________
std::map<int, std::vector<int> > proximity(std::map<int, std::vector<int> >&, std::map<int, std::vector<int> >&, int, bool, int);
// ____________________________________________________________
std::map<int, std::vector<int> > exact_phrase(std::vector<std::map<int, std::vector<int> > >&, bool);
// ____________________________________________________________
std::string search_rotate(std::string&);
// ____________________________________________________________
std::vector<std::string> star_retrieve(std::string&, std::map<std::string, std::string>&);
// ____________________________________________________________
std::vector<std::string> star_search(std::string&, std::map<std::string, std::string>&);
// ____________________________________________________________
std::vector<std::string> filter_words(std::vector<std::string>&, std::string&);