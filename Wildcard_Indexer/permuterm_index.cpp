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

std::vector<std::string> star_retrieve(std::string& s, PermutermIndex& pi)
{
	// find pointer to first element in range
	std::string sbeg = s.substr(0, s.size() - 1);
	PIpair begin = std::find_if(pi.begin(), pi.end(), [&](std::pair<std::string, std::string> scomp){ return scomp.first >= sbeg; });
	std::cout << "Word: " << sbeg << "\tFirst Element: " << begin->first << " => " << begin->second << std::endl;
	// find pointer to first element out of range
	std::string send = sbeg.substr(0, sbeg.size());
	send[send.size() - 1] = send.back() + 1;
	PIpair end = std::find_if(pi.begin(), pi.end(), [&](std::pair<std::string, std::string> scomp){ return scomp.first >= send; });
	std::cout << "Word: " << send << "\tLast Element: " << end->first << " => " << end->second << std::endl;
	// fill resulting vector with all values in range
	std::vector<std::string> result;
	for (auto it = begin; it != end; it++)
	{
		result.push_back(it->second);
	}
	return result;
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

	std::ofstream outpi("PermutermIndex");
  	{
    	boost::archive::binary_oarchive oa(outpi);
    	oa << pi;
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

std::vector<std::string> filter_words(std::vector<std::string>& vec, std::string& seq)
{
	std::vector<std::string> result;
	for (auto word: vec)
	{
		if (std::search(word.begin(), word.end(), seq.begin(), seq.end()) != word.end())
		{
			result.push_back(word);
		}
	}
	return result;
}

std::vector<std::string> star_search(std::string& s, PermutermIndex& pi)
{
	std::vector<std::string> result;
	int count = std::count(s.begin(), s.end(), '*');
	// if there is only one star, do normal star_retrieve
	if (count == 1)
	{
		std::string query = search_rotate(s);
		result = star_retrieve(query, pi);
	}
	// for two stars, we have to check different possible scenarios
	else if (count == 2)
	{
		if (s[0] == '*' && s.back() == '*')
		{
			// *w*
			std::string query(s.begin() + 1, s.end() - 1);
			query = query + "*";
			result = star_retrieve(query, pi);
		}
		else if (s[0] == '*')
		{
			// *w*w
			std::string sequence(s.begin() + 1, s.end());
			std::string::iterator sep = std::find(sequence.begin(), sequence.end(), '*');
			std::string first(sequence.begin(), sep);
			std::string second(sep, sequence.end());

			std::string query = search_rotate(second);
			result = star_retrieve(query, pi);
			result = filter_words(result, first);
		}
		else if (s.back() == '*')
		{
			// w*w*
			std::string sequence(s.begin(), s.end() - 1);
			std::string::iterator sep = std::find(sequence.begin(), sequence.end(), '*');
			std::string first(sequence.begin(), sep + 1);
			std::string second(sep + 1, sequence.end());

			std::string query = search_rotate(first);
			result = star_retrieve(query, pi);
			result = filter_words(result, second);
		}
		else
		{
			// w*w*w
			std::string::iterator sep1 = std::find(s.begin(), s.end(), '*');
			std::string::iterator sep2 = std::find(sep1 + 1, s.end(), '*');
			std::string first(s.begin(), sep1);
			std::string second(sep1 + 1, sep2);
			std::string third(sep2 + 1, s.end());

			std::string query = first + "*" + third;
			query = search_rotate(query);
			result = star_retrieve(query, pi);
			result = filter_words(result, second);
		}
	}
	return result;
}