// Copyright 2020, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// project headers
#include "./permuterm_index.h"

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

// this function takes the search term and rotates it s.t. 
// the star is at the end of the word.
std::string search_rotate(std::string& s)
{
	// for now we only consider examples with one star
	if (std::count(s.begin(), s.end(), '*') > 1)
	{
		std::string res;
		return res;
	}
	else
	{
		s.push_back('$');
		std::rotate(s.begin(), 
			std::find(s.begin(), s.end(), '*') + 1,
			s.end());
		return s;
	}
}


// function that reads an inverted index and constructs a permuterm index from it
PermutermIndex generatePI(std::ifstream& in)
{
	// deserialize Inverted Index
	InvertedIndex ii;
	{
		std::cout << "Inverted Index loading" << std::endl;
		boost::archive::binary_iarchive ia(in);
		ia >> ii;
	}
	std::cout << "Inverted Index loaded" << std::endl;

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

	return pi;
}

void runPI()
{
	std::ifstream in("InvertedIndex");
	PermutermIndex pi = generatePI(in);
	PermutermIndex::iterator step = pi.begin();
	PermutermIndex::iterator end = pi.end();
	unsigned int counter = 0;
	while (step != end)
	{
		std::cout << (step->first) << "\t" << (step->second) << "\n";
		step++; counter++;
	}
	std::cout << "\n\nCounter: " << counter << "\nGlobCounter: " << glob_counter << std::endl;
}
