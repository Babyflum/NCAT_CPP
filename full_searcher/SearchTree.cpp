// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlum.sadiku@anglistik.uni-freiburg.de>

// Implementation of Search Tree


// project headers
#include "./SearchTree.h"
#include "./Searcher.h"
#include "./RetrievePostings.h"

// standard headers
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <cctype>
#include <map>
#include <vector>


const char* ops[5] = {"AND", "OR", "NOT", "NEAR", "WITHIN"};
typedef std::map<int, std::vector<int> > postingsList;
typedef std::map<std::string, postingsList > InvertedIndex;
typedef std::map<std::string, std::string> PermutermIndex;


// empty constructor
TreeElement::TreeElement() : key(""), type(1) 
{
  this->parent = nullptr;
  this->left = nullptr;
  this->right = nullptr;
}

// constructor with just key and type
TreeElement::TreeElement(std::string key, int type) 
{
  this->key = key;
  this->type = type;
  this->parent = nullptr;
  this->left = nullptr;
  this->right = nullptr;
}

// constructor with no children
TreeElement::TreeElement(std::string key, int type, TreeElement* parent)
{
  this->key = key;
  this->type = type;
  this->parent = parent;
  this->left = nullptr;
  this->right = nullptr;
}

// full constructor
TreeElement::TreeElement(std::string key, int type, TreeElement* parent, TreeElement* left, TreeElement* right)
{
  this->key = key;
  this->type = type;
  this->parent = parent;
  this->left = left;
  this->right = right;
}

// methods
void TreeElement::printInfo(std::string s=0)
{
  std::cout << s + "Key: " << this->key << "\n" + s + "Type: " << this->type << "\n" + s + "Parent: ";
  if (this->parent == nullptr)
    std::cout << "nullptr";
  else
  {
    std::cout << this->parent->key;
  }
  std::cout << "\n" + s + "Left: ";
  if (this->left == nullptr)
  {
    std::cout << "nullptr";
  }
  else
  {
    std::cout << this->left->key;
  }
  std::cout << "\n" + s + "Right: ";
  if (this->right == nullptr)
  {
    std::cout << "nullptr";
  }
  else
  {
    std::cout << this->right->key;
  }
  std::cout << "\n\n" << std::endl;
}

void TreeElement::changeChildren(TreeElement* left, TreeElement* right)
{
  this->left = left;
  this->right = right;
}


// now we implement the SearchTree

// first the methods for tree generation

// this function will be used by generate to compare a term with the elements of the op-array
bool isOperator(std::string term) 
{
  /*
  first if the string is a within or a near
  we need to remove the numbers
  the preprocessor will make sure we can have at most 2 numbers.
  */
  std::string replace = term;
  int j = 0;
  while (j != 2) 
  {
    if (std::isdigit(replace.back())) 
    {
      replace = replace.substr(0, replace.size()-1);
    } 
    else 
      break;
    j++;
  }
  return std::any_of(std::begin(ops), std::end(ops), [&](std::string i) {return i == replace;});
}


void SearchTree::insertElement(std::string query_i, int type) 
{
  if (type == 4) 
  {
    if (current->left == nullptr) 
    {
      std::cout << "Opening Parenthesis: Go to left." << std::endl;
      TreeElement* t = new TreeElement;
      TreeElement t_local("", 1, current);
      *t = t_local;
      current->left = t;
      std::cout << "Change current pointer to left child." << std::endl;
      current = current->left;
    } 
    else
    {
      std::cout << "Opening Parenthesis: Go to right." << std::endl;
      TreeElement* t = new TreeElement;
      TreeElement t_local("", 1, current);
      *t = t_local;
      current->right = t;
      std::cout << "Change current pointer to right child." << std::endl;
      current = current->right;
    }
  } 
  else if (type == 5)
  {
    std::cout << "Closing Parenthesis: Go up.\nChange current pointer to parent." << std::endl; 
    current = current->parent;
  }
  else if (type == 3)
  {
    std::cout << "Operator: Change key of current node to " << query_i << std::endl;
    current->key = query_i;
    current->type = 3;
  }
  else
  {
    if (current->left == nullptr)
    {
      std::cout << "Single word or exact phrase: Go to left." << std::endl;
      TreeElement* t = new TreeElement;
      TreeElement t_local(query_i, type, current);
      *t = t_local;
      std::cout << "Change key of left child to " << query_i << std::endl;
      current->left = t;
    } 
    else
    {
      std::cout << "Single word or exact phrase: Go to right." << std::endl;
      TreeElement* t = new TreeElement;
      TreeElement t_local(query_i, type, current);
      *t = t_local;
      std::cout << "Change key of right child to " << query_i << std::endl;
      current->right = t;
    }
  }
}


void SearchTree::generate(std::vector<std::string>& query)
{
  size_t query_size = query.size();
  for (size_t i = 0; i != query_size; i++)
  {
    // first we define the type of term
    int typ;
    if (query[i] == "") 
      continue;
    if (::isOperator(query[i]))
    {
    	std::cout << query[i] << " is an Operator" << std::endl;
    	typ = 3;
    }
    else if (query[i] == "(")
    {
    	std::cout << query[i] << " is an Opening Parenthesis" << std::endl; 
    	typ = 4;
    }
    else if (query[i] == ")")
    {
    	std::cout << query[i] << " is an Closing Parenthesis" << std::endl; 
    	typ = 5;
    }
    else if (query[i][0] == '"')
    {
    	std::cout << query[i] << " is an exact phrase" << std::endl; 
    	typ = 2;
    }
    else if (std::any_of(query[i].begin(), query[i].end(), [&](char c){return c == '*';}))
    {
      std::cout << query[i] << " is a wildcard term" << std::endl;
      typ = 6;
    }
    else
    {
    	std::cout << query[i] << " is a single word" << std::endl; 
    	typ = 1;
    }
    // TreeElement t(query[i], typ);
    std::cout << "Inserting Element " << query[i] << " of type " << typ << std::endl;
    insertElement(query[i], typ);
  }
}


SearchTree::SearchTree()
{
  (this->root).key = "ROOT";
  (this->root).type = 1;
  (this->root).parent = nullptr;
  (this->root).left = nullptr;
  (this->root).right = nullptr;
  this->current = &root;
  this->pTrc = 0;
}

// debug function prints SearchTree
void SearchTree::printTree()
{
  std::string s(pTrc * 15, ' ');
  if (current->left == nullptr)
  {
    current->printInfo(s);
    return;
  }
  else
  {
    current->printInfo(s);
    pTrc++;
    current = current->left;
    printTree();
    current = current->parent->right;
    printTree();
    current = current->parent;
    pTrc--;
    return;
  }
}


// a helper function to split exact phrases into a vector of words.
std::vector<std::string> splitString(std::string& phrase)
{
  std::vector<std::string> result;
  phrase = phrase.substr(1, phrase.size() - 2);
  std::string::iterator beg = phrase.begin();
  std::string::iterator lim = beg;
  std::string::iterator end = phrase.end();
  while (lim != end)
  {
    if (std::isspace(*lim))
    {
      std::string temp(beg, lim);
      result.push_back(temp);
      lim++;
      beg = lim;
    } 
    else
      lim++;
  }
  std::string temp(beg, lim);
  result.push_back(temp);
  std::cout << "splitString() wants: " << phrase << std::endl;
  std::cout << "splitString() got: ";
  for (size_t i = 0; i != result.size(); i++) std::cout << result[i] << " ";
  std::cout << std::endl;
  return result;
}


// a helper function to determine the operator type, and 
// in the case of proximity operators, determine the dist value
int getOperatorType(std::string& op) {
  /*
  1 := AND; 2 := OR; 3 := NOT; 100 := NEAR; 200 := WITHIN;
  the dist value is the remaining two digits for near and within
  Example: 147 = N47.
  */
  std::cout << "getOperatorType called." << std::endl;
  if (op == "AND") 
    return 1;
  else if (op == "OR") 
    return 2;
  else if (op == "NOT") 
    return 3;
  else if (op[0] == 'N') 
  {
  	std::string t(op.begin() + 4, op.end());
  	int dist = std::stoi(t);
    std::cout << "NEAR is stored as " << t << " with distance " << dist << std::endl;
  	return (100 + dist);
  }
  else if (op[0] == 'W') 
  {
    std::string t(op.begin() + 6, op.end());
  	int dist = std::stoi(t);
    std::cout << "WITHIN is stored as " << t << " with distance " << dist << std::endl;
  	return (200 + dist);
  }
  else return 0;
}


// evaluate the tree
std::map<int, std::vector<int> > SearchTree::evaluate(InvertedIndex& ii, PermutermIndex& pi, bool leftbound) 
{
  std::cout << "Type of current is " << current->type << std::endl;
  std::cout << "Value of current is " << current->key << std::endl;
  if (current->type == 1) 
  {
    std::cout << current->key << " is recognized as single word." << std::endl;
    postingsList result = retrieve(current->key, ii);
    return result;
  } 
  else if (current->type == 2) 
  {
    std::cout << current->key << " is recognized as an exact phrase." << std::endl;
    // exact phrase: first we split the phrase into terms
    std::cout << "Exact Phrase is split up." << std::endl;
    std::vector<std::string> splitPhrase = ::splitString(current->key);
    // for each term, we retrieve the postingsList and push it onto a vector
    std::vector<postingsList> ep_input;
    std::cout << "Trying to retrieve all words of " << current->key << std::endl;
    for (size_t i = 0; i != splitPhrase.size(); i++) 
    {
      postingsList temp = ::retrieve(splitPhrase[i], ii);
      ep_input.push_back(temp);
    }
    // for the boolean value we also need to check if current is a left or right child.
    std::cout << "Calling exact_phrase(): " << std::endl;
    postingsList result = ::exact_phrase(ep_input, leftbound);
    std::cout << "exact_phrase() ran succesfully." << std::endl;
    return result;
  } 
  else if (current->type == 6)
  {
    postingsList result = ::wildcard_retrieve(current->key, ii, pi);
    return result;
  }
  else if (current->type == 3) 
  {
    std::cout << current->key << " is recognized as an operator." << std::endl;
    int op_type = ::getOperatorType(current->key);
    switch(op_type) 
    {
      case 1 : std::cout << current->key << " is recognized as AND" << std::endl; break;
      case 2 : std::cout << current->key << " is recognized as OR" << std::endl; break;
      case 3 : std::cout << current->key << " is recognized as NOT" << std::endl; break;
    }
    if (op_type > 99 && op_type < 200)
    {
      std::cout << current->key << " is recognized as NEAR with distance " << (op_type - 100) << std::endl;
    }
    else if (op_type > 199)
    {
      std::cout << current->key << " is recognized as WITHIN with distance " << (op_type - 200) << std::endl;
    }

    current = current->left;
    std::cout << "Evaluating left subtree." << std::endl;
    postingsList left = evaluate(ii, pi, true);
    std::cout << "Evaluating right subtree." << std::endl;
    current = current->parent->right;
    postingsList right = evaluate(ii, pi, false);
    std::cout << "Moving in with parents." << std::endl;
    current = current->parent;
    if (op_type == 1)
    {
      std::cout << "Calling intersect()" << std::endl;
    	postingsList result = ::intersect(left, right);
    	return result;
    } 
    else if (op_type == 2) 
    {
      std::cout << "Calling unionize()" << std::endl;
    	postingsList result = ::unionize(left, right);
    	return result;
    } 
    else if (op_type == 3) 
    {
      std::cout << "Calling complement()" << std::endl;
    	postingsList result = ::complement(left, right);
    	return result;
    } 
    else if (op_type >= 200) 
    {
    	// for the boolean value we also need to check if current is a left or right child.
    	int dist = op_type - 200;
      std::cout << "Calling proximity() with WITHIN" << std::endl;
    	postingsList result = ::proximity(left, right, dist, leftbound, 0);
    	return result;
    } 
    else if (op_type >= 100) 
    {
    	// for the boolean value we also neet to check if current is a left or right child.
    	int dist = op_type - 100;
      std::cout << "Calling proximity() with NEAR" << std::endl;
    	postingsList result = ::proximity(left, right, dist, leftbound, 1);
    	return result;
    } 
    else
    {
      std::cout << "Calling NO FUNCTION" << std::endl;
    	postingsList result;
    	return result;
    }
  } 
  else 
  {
  	std::cout << "BIG ERROR!!!!!!!" << std::endl;
  	std::map<int, std::vector<int> > result;
  	return result;
  }
}