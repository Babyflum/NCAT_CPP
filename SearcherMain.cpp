// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>
/*
_________________________________________
Main program that the search runs on.
Input of whatever query.
Return docIDs and (possibly) positions of matches.
__________________________________________________
1. Query is checked for legality.
2. Query is parsed and broken up into individual search items.
3. Query generates a SearchTree recursively.
4. At each stage two things happen:
4.1 SearchTree retrieves positional PostingsList for search item.
4.2 SearchTree computes the given operation on two lists.
5. Results are returned to Main.
6. Main retrieves documents from DB and sends them to the API.
7. API does its magic and shows user the documents with the
   search terms highlighted.
A. Must support multithreading.
B. Must support fast access to postings Lists.
C. Must minimize DISK-READs.
____________________________________________________
*/

// project headers
#include "./Searcher.h"
#include "./SearchTree.h"
#include "./RetrievePostings.h"

// standard headers
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cctype>

// boost headers
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>


typedef std::map<int, std::vector<int> > postingsList;
typedef std::map<std::string, postingsList> InvertedIndex;
typedef postingsList::iterator postIter;

std::vector<std::string> breakLine(std::string& query)
{
	std::vector<std::string> result;
	std::string::iterator beg = query.begin();
	std::string::iterator lim = beg;
	std::string::iterator end = query.end();
	while (lim != end)
	{
		if (std::isspace(*lim))
		{
			std::string temp(beg, lim);
			result.push_back(temp);
			lim++;
			beg = lim;
		} 
		else if (*lim == '(' || *lim == ')')
		{
			std::string temp(1, *lim);
			result.push_back(temp);
			lim++;
			beg = lim;
		}
	}
	return result;
}


int main()
{
	// load Index
	std::ifstream in("InvertedIndex");
	InvertedIndex ii;
	{
		boost::archive::binary_iarchive ia(in);
		ii << ia;
	}
	std::cout << "Inverted Index loaded\n" << std::endl;

	// ask user for query
	while (true)
	{
		std::cout << "THIS IS JUST A TEST SEARCH: ENTER ONLY WITH PROPER PARENTHESES!!!\n";
		std::cout << "Search: ";
		std::string input;
		std::getline(std::cin, input);
		if (input == "") break;
		std::cout << std::endl;

		// preprocess query (right now, we will only use breakline)
		std::vector<std::string> query = breakLine(input);

		// generate SearchTree
		SearchTree st;
		st.generate(query);
		postingsList result = st.evaluate(ii);
		if result.empty() std::cout << "Nothing to see here" << std::endl;
	}
	return 0;
}