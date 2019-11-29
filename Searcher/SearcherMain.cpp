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
#include "./Preprocessor.h"

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
typedef std::map<int, std::vector<std::string> > idMap;


void printQuery(std::vector<std::string>& query)
{
	std::cout << "[ ";
	for (size_t i = 0; i != query.size() - 1; i++)
	{
		std::cout << query[i] << ", ";
	}
	std::cout << query.back() << " ]" << std::endl;
}


// idMap transformer
std::string idMaptransformer(std::vector<std::string>& v)
{
	std::string temp;
	for (size_t i = 0; i != v.size(); i++)
	{
		temp = temp + v[i] + " ";
	}
	return temp;
}


int main()
{
	// load Index
	std::ifstream in("InvertedIndex");
	InvertedIndex ii;
	{
		std::cout << "Inverted Index loading" << std::endl;
		boost::archive::binary_iarchive ia(in);
		ia >> ii;
	}
	std::cout << "Inverted Index loaded\n" << std::endl;

	// load idMap
	std::ifstream inidMap("idMap");
	idMap im;
	{
		std::cout << "idMap loading" << std::endl;
		boost::archive::binary_iarchive ia(inidMap);
		ia >> im;
	}
	std::cout << "idMap loaded\n" << std::endl;

	// transform idMap into corpus
	typedef std::map<int, std::string> corpus;
	typedef corpus::iterator corit;
	typedef idMap::iterator idMapit;
	corpus Corpus;

	for (idMapit j = im.begin(); j != im.end(); j++)
	{
		Corpus[j->first] = idMaptransformer(j->second);
	}


	while (true)
	{
		std::cout << "Search: ";
		std::string input;
		std::getline(std::cin, input);
		if (input == "") break;

		// preprocess query (right now, we will only use breakline)
		std::cout << "Preprocessing Query" << std::endl;
		std::vector<std::string> processed_query = preprocessor::run(input);
		printQuery(processed_query);

		postingsList result;

		if (processed_query.empty())
		{
			std::cout << "Something went wrong.\n" << std::endl;
			continue;
		}
		else if (processed_query.size() == 1)
		{
			processed_query.push_back("OR");
			processed_query.push_back(" ");
		}
		// generate SearchTree
		SearchTree st;
		std::cout << "\nGenerating Search Tree" << std::endl;
		st.generate(processed_query);
		std::cout << "\nEvaluating search tree" << std::endl;
		result = st.evaluate(ii);
		if (result.empty()) std::cout << "Nothing to see here" << std::endl;
		else
		{ 
			std::cout << "Found something" << std::endl;
			int counter = 0;
			postIter postbegit = result.begin();
			postIter postendit = result.end();

			while (counter != 5 && postbegit != postendit)
			{
				std::cout << "\n" << Corpus[postbegit->first] << std::endl;
				std::cout << "\n" << im[postbegit->first][0] << std::endl;
				counter++;
				postbegit++;
			}
		}
	}
	std::cout << "\nCleaning up!\nThank you for using NCAT" << std::endl;
	return 0;
}