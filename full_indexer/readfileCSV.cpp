// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// project headers
#include "readfileCSV.h"

// standard headers
#include <vector>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

// boost headers
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>


typedef std::vector<std::vector<std::string> > lineVec;
typedef std::map<int, std::vector<std::string> > idMap;
typedef std::map<int, std::string> database;


// from the list element read the line and split it into its columns
std::vector<std::string> lineToVec(std::string& str) 
{
  std::vector<std::string> result;
  /* 
  line starts with \" and ends with \" and individual
  columns are separated by " ," so we ignore the first and
  last element 
  */
  std::string::iterator iter = str.begin() + 1;
  std::string::iterator begin = iter;		// set iterator for every column
  
  while (iter != str.end() - 1) 
  {
    if (*iter == '"' && *(iter+1) == ',' && *(iter+2) == '"')
    {
      std::string temp(begin, iter);
      result.push_back(temp);
      iter = iter + 3;
      begin = iter;
    } 
    else
      iter++;
  }
  std::string temp(begin, iter);
  result.push_back(temp);
  return result;
}


/* 
read a .csv file and store the lines as elements in a list
the elements are vectors containing the columns as elements
this uses the lineToVec function 
*/
lineVec readFile(std::ifstream& in) 
{
  lineVec result;
  std::string line;
  int counter = 0;
  
  while (std::getline(in, line)) 
  {
    if (line[0] == '\"') 
    {
      std::vector<std::string> ret = lineToVec(line);
      result.push_back(ret);
      counter++;
    } 
    else
      continue;
  }
  std::cout << "Counter: " << counter << std::endl;
  return result;
}


// return true if char is a separator
bool isSeparator(const char c)
{
  // string containing an amalgam of possible separators to be extended if needed
  static std::string separator = " .?!:;&\\/()";
  std::string::const_iterator iter = std::find(separator.begin(), separator.end(), c);
  if (iter != separator.end()) 
    return true;
  else 
    return false;
}


bool isAcronym(std::string::iterator cp) 
{
  if (*cp == '.') 
  {
    if (std::isalpha(*(cp-1)) && std::isalpha(*(cp+1)) && !std::isalpha(*(cp+2))) 
    {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}


// split the words in the postContent column into a vector which is then used as the value to the idMap
std::vector<std::string> split(std::string& str) 
{
  std::vector<std::string> result;
  typedef std::string::iterator striter;
  striter begin = str.begin();
  striter it = begin;
  int counter = 0;

  while (it != str.end()) 
  {
    // check for separators like empty space and .
    if (it != str.begin() && isSeparator(*it)) 
    {
      std::string temp(begin, it++);
      result.push_back(temp);
      // std::cout << "Word #: " << ++counter << std::endl; set begin to first letter of next word after space
      while (it != str.end() && !std::isalnum(*it))
        it++;
      begin = it;
    } 
    else if (*it == ',') 
    {
      // check if there are numbers around the comma this indicates a number like 42,000
      if (it != str.end() && std::isdigit(*(it-1)) && std::isdigit(*(it+1)))
        it++;
      else 
      {
        std::string temp(begin, it++);
        result.push_back(temp);
        // std::cout << "Word #: " << ++counter << std::endl;
        while (it != str.end() && !std::isalnum(*it))
          it++;
        begin = it;
      }
    } 
    else 
      it++;
  }
  return result;
}


/* 
generate a map that contains the docIDs as keys (converted to int) and the
words in the doc as a container with the elements being the words
needs a split function that separates all words.
*/
idMap genMap(lineVec& lv, database& DB) 
{
  idMap result;
  typedef lineVec::iterator lviter;

  for (lviter it = lv.begin(); it != lv.end(); it++) 
  {
    int n = std::stoi((*it)[0]);
    DB[n] = (*it)[8];
    std::vector<std::string> vec = split((*it)[8]);
    result[n] = vec;
  }
  return result;
}


/* 
read a CSV file containing the raw data
the input is the name of the CSV file
return an idMap containing the docIDs as keys
and vectors with words per docID as elements.
*/
idMap RunMain(std::string name)
{
  database DB;

  std::ifstream infile;
  infile.open(name);
  lineVec lines;

  if (infile.is_open()) 
  {
    lines = readFile(infile);
    infile.close();
  } 
  else 
  {
    throw std::domain_error("infile doesn't exist");
  }

  idMap result = genMap(lines, DB);

  std::ofstream outDB("database");

  {
    boost::archive::binary_oarchive oa(outDB);
    oa << DB;
  }
  return result;
}