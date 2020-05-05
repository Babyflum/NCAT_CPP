// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

/*
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
This module provides functions that take two positional Postings Lists
and combines them according to the search operators returning one
resulting list.

The functions include:
INTERSECT (AND of two lists with just docIDs)
UNION (OR of two lists with just docIDs)
NOT (COMPLEMENT of second list with regards to first
     meaning all elements in the first list that do not
     appear in the second list, again only docIDs)
PROXIMITY (AND of two lists with additional restriction that
           WITHINx for a number x > 0 denotes the second word
	   following the first word by at most x positions,
	   NEARx for a number x > 0 denotes the second word
	   being at most x positions to the left or right of
	   the first word, and EXACTx for a number x > 0
	   denotes the second word being exactly x positions
	   to the right of the first word. Note that there is
	   no option to have the second word exactly x positions
	   to the left of the first word. This is deemed unnecessary
	   since one can simply switch the order of words in the query)
EXACT_PHRASE (AND of two lists with additional restriction that the
              search terms within the phrase need to match to consecutive
	      integers in terms of positions)

The functions might be expanded to include more options for search.
A primary focus for the future is the possibility of a wildcard (or star)
search, as well as a part-of-speech search.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/


// project headers
#include "./Searcher.h"
#include "./RetrievePostings.h"

// standard headers
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>


typedef std::map<int, std::vector<int> > postingsList;
typedef std::map<std::string, postingsList > InvertedIndex;
typedef postingsList::iterator postIt;
typedef std::vector<int>::iterator VecIt;
typedef std::map<std::string, std::string> PermutermIndex;
typedef PermutermIndex::iterator PIpair;

/* 
this function is used by the BOOLEAN functions and merges 
the two position vectors of docIDs matching the condition.
this assumes the fact that two positions for the same docID
cannot be identical.
*/
std::vector<int> mergePos(std::vector<int>& left, std::vector<int>& right) 
{
  std::vector<int> result;
  VecIt leftIt = left.begin();
  VecIt rightIt = right.begin();
  while (leftIt != left.end() && rightIt != right.end()) 
  {
    if (*leftIt < *rightIt) 
    {
      result.push_back(*leftIt);
      leftIt++;
    } 
    else 
    {
      result.push_back(*rightIt);
      rightIt++;
    }
  }
  if (leftIt == left.end()) 
    result.insert(result.end(), rightIt, right.end());
  else 
    result.insert(result.end(), leftIt, left.end());
  return result;
}	


// return a postingsList that is the AND of the two input lists.
postingsList intersect(postingsList& left, postingsList& right) 
{
  postingsList result;
  postIt leftIt = left.begin();
  postIt rightIt = right.begin();
  while (leftIt != left.end() && rightIt != right.end()) 
  {
    if (leftIt->first == rightIt->first) 
    {
      result[leftIt->first] = mergePos(leftIt->second, rightIt->second);
      leftIt++;
      rightIt++;
    } 
    else if (leftIt->first < rightIt->first)
      leftIt++;
    else
      rightIt++;
  }
  return result;
}


// return a postingsList that is the OR of the two input lists.
postingsList unionize(postingsList& left, postingsList& right) 
{
  postingsList result;
  postIt leftIt = left.begin();
  postIt rightIt = right.begin();
  while (leftIt != left.end() && rightIt != right.end())
  {
    if (leftIt->first == rightIt->first) 
    {
      result[leftIt->first] = mergePos(leftIt->second, rightIt->second);
      leftIt++;
      rightIt++;
    } 
    else if (leftIt->first < rightIt->first)
    {
      result[leftIt->first] = leftIt->second;
      leftIt++;
    } 
    else 
    {
      result[rightIt->first] = rightIt->second;
      rightIt++;
    }
  }

  if (leftIt == left.end()) 
  {
    while (rightIt != right.end()) 
    {
      result[rightIt->first] = rightIt->second;
      rightIt++;
    }
  } 
  else if (rightIt == right.end())
  {
    while (leftIt != left.end())
    {
      result[leftIt->first] = leftIt->second;
      leftIt++;
    }
  }
  return result;
}


// return a postings List that is the NOT of the two input lists.
postingsList complement(postingsList& left, postingsList& right)
{
  postingsList result;
  postIt leftIt = left.begin();
  postIt rightIt = right.begin();
  while (leftIt != left.end() && rightIt != right.end())
  {
    if (leftIt->first < rightIt->first) 
    {
      result[leftIt->first] = leftIt->second;
      leftIt++;
    }
    else if (leftIt->first > rightIt->first)
      rightIt++; 
    else
    {
      leftIt++;
      rightIt++;
    }
  }

  while (leftIt != left.end()) 
  {
    result[leftIt->first] = leftIt->second;
    leftIt++;
  }
  return result;
}


/* 
return a postingsLists which contains only those docIDs where the left word
and the right word are in proximity to each other, specified by a number, e.g.
NEAR7, WITHIN5
*/
postingsList proximity(postingsList& left, postingsList& right, int dist, bool leftbound, int op=0) 
{
  postingsList result;
  // The docIDList contains all docIDs such that the left and right terms are in those docIDs
  postingsList docIDList = intersect(left, right);
  // if left and right don't share docIDs, return an empty postingsList.
  if (docIDList.empty())
    return docIDList;
  
  // if operator is 0, apply WITHINdist
  if (op==0) 
  {
    for (postIt pit = docIDList.begin(); pit != docIDList.end(); pit++)
    {
      VecIt pl = left[pit->first].begin();
      VecIt pr = right[pit->first].begin();
      while (pl != left[pit->first].end() && pr != right[pit->first].end()) 
      {
        if (*pr < *pl)
          pr++;
        else if (*pr-*pl > dist)
        {
          pl++;
        }
        else 
        {
          leftbound ? result[pit->first].push_back(*pr) : result[pit->first].push_back(*pl);
          pl++; 
          pr++;
        } 
      }
    }
  
  // if operator is 1, apply NEARdist
  } 
  else if (op==1)
  {
    for (postIt pit = docIDList.begin(); pit != docIDList.end(); pit++)
    {
      VecIt pl = left[pit->first].begin();
      VecIt pr = right[pit->first].begin();
      while (pl != left[pit->first].end() && pr != right[pit->first].end())
      {
        if (*pr > *pl + dist)
          pl++;
        else if (*pl > *pr + dist)
          pr++;
        else
        { 
          leftbound ? result[pit->first].push_back(std::max(*pl, *pr)) : result[pit->first].push_back(std::min(*pl, *pr));
          pl++; 
          pr++;
        }
      }
    }
  }
  return result;
}


/*
This function returns an exact phrase match of a variable number of postingsList (l1, l2,...,ln)
if and only if for corresponding docIDs there exist positions in l1, l2,...,ln, such that
p(lk) + 1 == p(lk+1) for all k in {1,..,n}.
*/
postingsList exact_phrase(std::vector<postingsList>& phrase, bool leftbound)
{
  postingsList result;
  // first we need to check whether the individual terms even exist in the same document.
  const size_t n = phrase.size();
  postingsList docIDList = intersect(phrase[0], phrase[1]);
  size_t current = 2;
  while (current != n)
    docIDList = intersect(docIDList, phrase[current++]);
  if (docIDList.empty())
  {
    std::cout << "No intersection for exact phrase." << std::endl;
    return result;
  }
  // if we have matches in docID, we can begin the algorithm
  std::cout << "Begin crazy exact phrase algorithm" << std::endl;
  for (postIt pit = docIDList.begin(); pit != docIDList.end(); pit++)
  {
    // pit refers to the docIDs in the intersection.
    unsigned int p = 0;
    // p is the position in the intersect-vector for a particular docID
    while (p+n <= (pit->second).size() && p <= (pit->second).size()) 
    {
      if ((pit->second)[p] + (n-1) == (pit->second)[p+n-1])
      {
        // if this is the case check if the phrase actually matches and is not just a permutation
        bool match = true;
        // k is the index of the phrase, phrase[k] means the kth postingsList.
        unsigned int k = 0;
        while (match && (k != n)) 
        {
        // Search through the positional lists for all terms in the phrase for each docID. If the numbers don't exist, the match is false.
          if ((std::find(phrase[k][pit->first].begin(), phrase[k][pit->first].end(), (pit->second)[p+k])) == phrase[k][pit->first].end()) 
            match = false;
          k++;
        }
        if (match)
        {
          leftbound ? result[pit->first].push_back((pit->second)[p+n-1]) : result[pit->first].push_back((pit->second)[p]); p += n;
        }
        else 
          p++;
      } 
      else 
        p++;
    }
  } 
  return result;
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

postingsList wildcard_retrieve(std::string& wildcard_term, InvertedIndex& ii, PermutermIndex& pi)
{
  // retrieve all matching terms from the permuterm index  
  std::vector<std::string> wildcard_matches = star_search(wildcard_term, pi);
  // combine docIDs for all matches
  postingsList result;
  for (int i = 0; i != wildcard_matches.size(); i++)
  {
    postingsList loop = retrieve(wildcard_matches[i], ii);
    result = unionize(loop, result);
  }
  return result;
}