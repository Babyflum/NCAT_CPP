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
int ratio_counter = 0;

// this function gives the length of the longest set of consecutive letters
int consecutive_letters(std::string& s)
{
	int longest = 0;
	int current = 1;
	std::string::iterator step = s.begin();
	char compare = '$'; 

	while (step != s.end())
	{
		if (*step == compare)
		{
			current++;
		}
		else
		{
			if (current > longest)
			{
			    longest = current;
			}
			compare = *step;
			current = 1;
		}
		step++;
	}
	if (current > longest)
	{
		longest = current;
	}
	return longest;
}

// function that checks whether term is a real word or not
bool wordIsValid(std::string& s)
{
	// no numbers are allowed
	if (std::any_of(s.begin(), s.end(), [](char c){return std::isdigit(c);}))
	{
		return false;
	}
	// no more than two consecutive letters are the same
	else if (consecutive_letters(s) > 2)
	{
		return false;
	}
	// characters are either alphabetical or hyphens
	else if (std::any_of(s.begin(), s.end(), [](char c){return !(std::isalpha(c) || c == '-');}))
	{
		return false;
	}
	// if we have a hyphen, it cannot be at the beginning or the end of a word
	else if (!(std::isalpha(s[0]) && std::isalpha(s[s.size() - 1])))
	{
		return false;
	}
	return true;
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
			std::cout << "Term is valid" << std::endl;
			std::string str = term + "$";
			pi[str] = term;
			while (*(str.begin()) != '$')
			{
				str = permuterm_rotate(str);
				pi[str] = term;
			}
			ratio_counter++;
		}
		step++;
	}

	std::ofstream outpi("PermutermIndex1M");
  	{
    	boost::archive::binary_oarchive oa(outpi);
    	oa << pi;
  	}
  	float ratio = (ratio_counter/glob_counter) * 100;
  	std::cout << "Permuterm Index contains " << ratio << " % of all terms" << std::endl;

	return pi;
}