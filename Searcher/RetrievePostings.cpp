// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// project headers
#include "./RetrievePostings.h"

// standard headers
#include <vector>
#include <string>
#include <map>
#include <iostream>


typedef std::map<int, std::vector<int> > postingsList;
typedef std::map<std::string, postingsList> InvertedIndex;

// ________________________________________________________
postingsList retrieve(std::string& term, InvertedIndex& ii) 
{
	postingsList empty;
	if (ii.find(term) == ii.end())
		std::cout << "Term cannot be found\n" << std::endl;
	else 
		return ii[term];
}