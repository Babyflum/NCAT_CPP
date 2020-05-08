// Copyright 2018, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// standard headers
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <cctype>


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


/*
This function is supposed to be malleable according to the needs of the corpus.
The basic idea is to decide whether including a string in the inverted index
is beneficial to the goal of information retrieval. Excluded terms do not change
the positional indexing. However, they do not appear in the inverted index and therefore
cannot be searched for , making the index leaner by preventing bloat.
Version 1.x of this program has the following general rules:
1) 	The word shall not be more than 30 letters long.
	This would exclude dictionary words like pseudopseudohypoparathyrodism
	but also make sure that sentences with no space in between words, as well
	as nonce words above a certain length are not included.
2)	The word shall not consist of only numbers longer than 4 digits.
	This makes sure that years and other dates are kept, while removing
	random numbers.
3)	If the word contains letters and numbers, it shall not contain more
	than 2 consecutive digits. This would include words like 4eva, h8 and
	69ing, but would exclude any random letter-number-combinations.
4) 	If the word contains letters and numbers, it shall not be longer than 10
	characters.
5)  The word shall not contain the same letters more than twice in a row.
If a term conforms to these rules, it will be considered interesting.
*/
bool interesting_term(std::string& str)
{
	bool interesting = true;
	if (str.size() > 29)
	{
		interesting = false;
	}
	else if (std::all_of(str.begin(), str.end(), [](char c){return std::isdigit(c);}))
	{
		if (str.size() > 4)
		{
			interesting = false;
		}
	}
	else if (std::any_of(str.begin(), str.end(), [](char c){return std::isdigit(c);}))
	{
		if (str.size() > 10)
		{
			interesting = false;
		}
		else if (std::count_if(str.begin(), str.end(), [](char c){return std::isdigit(c);}) > 2)
		{
			interesting = false;
		}
	}
	else
	{
		interesting = true;
	}
	return interesting;
}


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
			// check whether the string is an interesting word
			if (interesting_term(term))
			{
				// change to lowercase
				std::transform(term.begin(),
					term.end(),
					term.begin(),
					[](unsigned char c) { return std::tolower(c); });
				ii[term][docID].push_back(pos);
			}
			pos += 1;
		}
	}
  return ii;
}