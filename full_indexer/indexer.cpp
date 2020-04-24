// Copyright 2018, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// standard headers
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>


//define the inverted index structure
typedef std::map<std::string, std::map<int, std::vector<int> > > InvertedIndex;
//define an iterator for the inverted index
typedef InvertedIndex::iterator iiIter;
//define an iterator for the docIDMap
typedef std::map<int, std::vector<int> >::iterator docIDIter;

//define the idMap structure
typedef std::map<int, std::vector<std::string> > IDMAP;
//define an iterator for the IDMAP
typedef IDMAP::iterator idMapIter;


InvertedIndex generateIndex(IDMAP& idMap) 
{
// generate an inverted Index from an idMap;
	InvertedIndex ii;

	for (idMapIter MI = idMap.begin(); MI != idMap.end(); MI++) 
	{
		int docID = MI->first;
		int pos = 0;
		for (int i = 0; i != (MI->second).size(); i++) 
		{
			std::string term = (MI->second)[i];
			ii[term][docID].push_back(pos);
			pos += 1;
		}
	}
  return ii;
}