// Copyright 2020, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// project headers
#include "./permuterm_index.h"

// standard headers
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

typedef std::map<std::string, std::string> PermutermIndex;
typedef PermutermIndex::iterator PIpair;



int main()
{
	// runPI();
	std::ifstream in("InvertedIndex");
	PermutermIndex pi = generatePI(in);
	
	while (true)
	{
		std::string what;
		std::cin >> what;
		if (what == "") break;
		// std::string what = "de*s*ing";
		// std::string query = search_rotate(what);
		// std::vector<std::string> result = star_retrieve(query, pi);
		std::vector<std::string> result = star_search(what, pi);
		for (auto word: result) std::cout << word << std::endl;
	}
	/*
	std::vector<std::string> testkeys = {"jesus", "aard", "chernobyl", "aardvark", "cherno"};
	std::vector<std::string> testvalues = {"this", "This", "than", "is", "smaller"};
	PermutermIndex testindex;

	for (size_t i = 0; i != testkeys.size(); i++)
	{
		testindex[testkeys[i]] = testvalues[i];
	}

	// std::vector<std::string>::iterator found;
	// found = std::find_if(testkeys.begin(), testkeys.end(), [&](std::pair<std::string, std::string> s){ return s.first >= "cher"; });
	// std::cout << *found << std::endl;

	// we try to find all elements that match cher*
	PIpair begin, end;
	begin = std::find_if(testindex.begin(), testindex.end(), [&](std::pair<std::string, std::string> s){ return s.first >= "cher"; });
	end = std::find_if(testindex.begin(), testindex.end(), [&](std::pair<std::string, std::string> s){ return s.first >= "ches"; });
	// we should get cherno and chernobyl
	for (auto ptr = begin; ptr != end; ptr++)
	{
		std::cout << ptr->first << " " << ptr->second << std::endl;
	}

	PIpair begin, end;
	std::pair<PIpair, PIpair> equal_ptr;
	begin = pi.lower_bound("n$m");
	end = pi.upper_bound("m");
	equal_ptr = pi.equal_range("n$m");

	if (end != pi.end()) std::cout << "FUCKYOU!" << std::endl;
	if (begin != pi.end()) std::cout << "FUCKYOU2222!" << std::endl;
	if (equal_ptr.first != pi.end()) std::cout << "FUCKYOU3333!" << std::endl;

	for (auto it = begin; it != end; it++)
	{
		std::cout << it->first << std::endl;
	}
	*/



	return 0;
}