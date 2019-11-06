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
	std::cout << "retrieve() is trying to retrieve " << term << std::endl;
	postingsList empty;
	if (ii.find(term) == ii.end())
	{
		std::cout << "Term cannot be found\n" << std::endl;
		return empty;
	}
	else
	{
		std::cout << "Term was succesfully retrieved." << std::endl;
		return ii[term];
	}
}
