// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>

// project headers
#include "readfileCSV.h"
#include "indexer.h"

// standard headers
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <cctype>
#include <chrono>
#include <fstream>

// boost headers
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/vector.hpp>


int main() 
{
  auto start = std::chrono::high_resolution_clock::now();

  std::map<int, std::vector<std::string> > idMap;
  try 
  { 
    idMap = RunMain("post_0-1M.csv");
  } 
  catch (const std::domain_error e) 
  {
    std::cout << "Caught an error" << std::endl;
    return 1;
  }
  
  typedef std::map<std::string, std::map<int, std::vector<int> > > InvertedIndex;
  // typedef InvertedIndex::iterator iiIter;
  // typedef std::vector<int>::iterator VecIt;

  // this is where we generate the index from the idMap
  InvertedIndex ii = generateIndex(idMap);
  
  // DON'T destruct idMap
  // idMap.~map();

  auto end = std::chrono::high_resolution_clock::now();

  std::chrono::duration<float> duration = end - start;
  std::cout << duration.count() << "s " << std::endl;

  // serialize index to binary file
  std::ofstream out("InvertedIndex");

  {
    boost::archive::binary_oarchive oa(out);
    oa << ii;
  }

  return 0;
}