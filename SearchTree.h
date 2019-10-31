// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlum.sadiku@anglistik.uni-freiburg.de>

// Header file for Search Tree

#pragma once


// standard headers
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <string>


// forward declaration
class SearchTree;

class TreeElement 
{
 friend class SearchTree;
 // attributes
 private:
  std::string key;
  int type;  // 1: single term, 2: exact phrase, 3: operator, 4: opening parenthesis, 5: closing parenthesis
  TreeElement* parent;
  TreeElement* left;
  TreeElement* right;

 // constructors
 public:
  // empty constructor
  TreeElement();
  // constructor with just key and type, default for adjacent nodes is NULL
  TreeElement(std::string, int);
  // construct a child with no children
  TreeElement(std::string, int, TreeElement*);
  // full constructor
  TreeElement(std::string, int, TreeElement*, TreeElement*, TreeElement*);

 // methods
 public:
  // print information about the TreeElement
  void printInfo(std::string);
  // change values of left and right children
  void changeChildren(TreeElement*, TreeElement*);
};


class SearchTree {
 friend class TreeElement;

 // attributes
 private:
  TreeElement root;
  TreeElement* current;
 
 // constructors
 public:
  SearchTree();
 
 // private methods.
 private:
  // insert a TreeElement relative to current
  void insertElement(std::string query_i, int type);
 
 // public methods
 public:
  // debug tool to print tree in readable format, pTrc := pritnTreerecursioncounter
  int pTrc;
  void printTree();
  // fill an empty tree with an inputVector
  // this method is used for the inputVector-Constructor.
  void generate(std::vector<std::string>&);
  // method to get a positionalPostingsList according to the operator hierarchy
  std::map<int, std::vector<int> > evaluate(std::map<std::string, std::map<int, std::vector<int> > >&);
};

// declaration of helper functions
int getOperatorType(std::string&);
std::vector<std::string> splitString(std::string&);
bool isOperator(std::string term);