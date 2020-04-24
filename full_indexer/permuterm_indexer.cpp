// Copyright 2020, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// project headers
#include "./permuterm_indexer.h"

// standard headers
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <algorithm>

// boost headers
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>

// typedefs
typedef std::map<int, std::vector<int> > postingsList;
typedef std::map<std::string, postingsList> InvertedIndex;
typedef std::map<std::string, std::string> PermutermIndex; // maps a permuterm to a term
typedef PermutermIndex::iterator PIpair;


int glob_counter = 0;

// function that checks whether term is a real word or not
bool wordIsValid(std::string& s)
{
	// false if word is longer than 30 characters
	if (s.size() > 30)
	{
		return false;
	}
	// false if word contains only numbers
	else if (std::all_of(s.begin(), s.end(), [](unsigned char c){return std::isdigit(c);}))
	{
		return false;
	}
	// false if word contains any non-letter characters except the hyphen
	else if (std::any_of(s.begin(), s.end(), [](unsigned char c){return !(std::isalnum(c) ||  c == '-');}))
	{
		return false;
	}
	else return true;
}


// this function does one move of the rotation
std::string permuterm_rotate(std::string s)
{
	std::rotate(s.begin(), s.begin() + 1, s.end());
	return s;
}


// function that reads an inverted index and constructs a permuterm index from it
PermutermIndex generatePI(InvertedIndex& ii)
{
	// generate Permuterm Index
	PermutermIndex pi;

	InvertedIndex::iterator step = ii.begin();
	InvertedIndex::iterator end = ii.end();
	while (step != end)
	{
		glob_counter++;
		std::string term = (step->first);
		// first we collapse casing
		std::transform(term.begin(),
			term.end(),
			term.begin(),
			[](unsigned char c) { return std::tolower(c); });
		// check if word is valid
		if (!wordIsValid(term))
		{
			step++;
			continue;
		}
		else
		{
			std::string str = term + "$";
			pi[str] = term;
			while (*(str.begin()) != '$')
			{
				str = permuterm_rotate(str);
				pi[str] = term;
			}
		}
		step++;
	}

	std::ofstream outpi("PermutermIndex");
  	{
    	boost::archive::binary_oarchive oa(outpi);
    	oa << pi;
  	}

	return pi;
}