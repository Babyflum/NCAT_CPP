// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>


// standard header includes
#include <string>
#include <vector>
#include <cctype>
#include <iostream>
#include <algorithm>


#define PRINT	std::cout << "Counter: " << counter \
	<< "\nValue of step: " << *step << "\n" << \
	"Value of beg: " << *beg << "\n" << "Value of afterword: " << \
	afterword << "\n" << "Value of afterwhitespace: " << afterwhitespace \
	<< "\n"; for(size_t i=0; i != result.size(); i++) {std::cout << result[i];} std::cout \
	<< "\n" << std::endl;


bool isEndWord(const char c)
{
	char test[6] = {'(', ')', ',', '&', '|', '~'};
	char *foo = std::find(std::begin(test), std::end(test), c);
	return (foo != std::end(test));
}

namespace preprocessor 
{
	std::string setOperators(std::string& query)
	{
		std::string result;
		std::string::iterator step = query.begin();  // this iterator marks the character we are currently ar
		std::string::iterator beg = step;  // this iterator marks the beginning of a word
		std::string::iterator end = query.end();  // this iterator marks the end of the query
		bool afterword = false;
		bool afterwhitespace = false;
		int counter = 0;

		// we go through the entire query until we reach the end
		while (step != end)
		{
			counter++;
			// PRINT
			// check whether whitespace is AND or just whitespace
			if (*step == ' ')
			{
				step++;
				beg = step;
				afterwhitespace = true;
				continue;
			}  // END whitespace

			// check for parentheses.
			else if (*step == '(')
			{
				result.append(" ( ");
				afterword = false;
				afterwhitespace = false;
				step++;
				beg = step;
				continue;
			}
			else if (*step == ')')
			{
				result.append(" ) ");
				afterword = false;
				afterwhitespace = false;
				step++;
				beg = step;
				continue;
			}  // END parentheses

			// check for exact phrase.
			else if (*step == '"')
			{
				beg = step;  // beg is now at '"'
				step++;
				while (*step != '"' && step != end)
				{
					step++;
				}
				// after while, step is at second '"';
				step++;
				if (afterwhitespace == true && afterword == true)
				{
					std::string temp(beg, step);
					temp = " AND " + temp + " ";
					result.append(temp);
					afterwhitespace = false;
					afterword = true;
					beg = step;
					continue;
				}
				else
				{
					std::string temp(beg, step);
					temp = " " + temp + " ";
					result.append(temp);
					afterwhitespace = false;
					afterword = true;
					beg = step;
					continue;
				}
			}  // END exact phrase

			// check for all single character operators.
			else if (*step == '&')
			{
				result.append(" AND ");
				afterword = false;
				afterwhitespace = false;
				step++;
				beg = step;
				continue;
			}
			else if (*step == ',' || *step == '|')
			{
				result.append(" OR ");
				afterword = false;
				afterwhitespace = false;
				step++;
				beg = step;
				continue;
			} 
			else if (*step == '~')
			{
				result.append(" NOT ");
				afterword = false;
				afterwhitespace = false;
				step++;
				beg = step;
				continue;
			}  // END single character operators

			// check AND, OR, NOT
			else if (*step == 'A' && *(step+1) == 'N' && *(step+2) == 'D')
			{
				result.append(" AND ");
				afterword = false;
				afterwhitespace = false;
				step += 3;
				beg = step;
				continue;
			}
			else if (*step == 'O' && *(step+1) == 'R')
			{
				result.append(" OR ");
				afterword = false;
				afterwhitespace = false;
				step += 2;
				beg = step;
				continue;
			}
			else if (*step == 'N' && *(step+1) == 'O' && *(step+2) == 'T')
			{
				result.append(" NOT ");
				afterword = false;
				afterwhitespace = false;
				step += 3;
				beg = step;
				continue;
			}  // END AND, OR, NOT

			// check NEARd and WITHINd
			else if (*step == 'N')
			{
				// N with digits after it
				if (std::isdigit(*(step+1)))
				{
					// N with 2 digits after it
					if (std::isdigit(*(step+2)))
					{
						step += 3;
						std::string temp(beg + 1, step);
						temp = " NEAR" + temp + " ";
						result.append(temp);
						beg = step;
						afterword = false;
						afterwhitespace = false;
						continue;
					}
					// N with 1 digit after it
					else
					{
						step += 2;
						std::string temp(beg + 1, step);
						temp = " NEAR" + temp + " ";
						result.append(temp);
						beg = step;
						afterword = false;
						afterwhitespace = false;
						continue;
					}
				}
				// NEAR wth digits after it
				else if (*(step+1) == 'E' && *(step+2) == 'A' && *(step+3) == 'R')
				{
					if (std::isdigit(*(step+4)))
					{
						// NEAR with 2 digits after it
						if (std::isdigit(*(step+5)))
						{
							step += 6;
							std::string temp(beg, step);
							temp = " " + temp + " ";
							result.append(temp);
							beg = step;
							afterword = false;
							afterwhitespace = false;
							continue;
						}
						// NEAR with 1 digit after it
						else
						{
							step += 5;
							std::string temp(beg, step);
							temp = " " + temp + " ";
							result.append(temp);
							beg = step;
							afterword = false;
							afterwhitespace = false;
							continue;
						}
					}
					else
					{
						step += 4;
						std::string temp(beg, step);
						temp = " " + temp + "1 ";
						result.append(temp);
						beg = step;
						afterword = false;
						afterwhitespace = false;
						continue;
					}
				}
			}  // END NEARd

			else if (*step == 'W')
			{
				if (std::isdigit(*(step+1)))
				{
					// W with 2 digits after it
					if (std::isdigit(*(step+2)))
					{
						step += 3;
						std::string temp(beg + 1, step);
						temp = " WITHIN" + temp + " ";
						result.append(temp);
						beg = step;
						afterword = false;
						afterwhitespace = false;
						continue;
					}
					// W with 1 digit after it
					else
					{
						step += 2;
						std::string temp(beg + 1, step);
						temp = " WITHIN" + temp + " ";
						result.append(temp);
						beg = step;
						afterword = false;
						afterwhitespace = false;
						continue;
					}
				}
				// WITHIN with digits after it
				else if (*(step+1) == 'I' && *(step+2) == 'T' && *(step+3) == 'H' && *(step+4) == 'I' && *(step+5) == 'N')
				{
					if (std::isdigit(*(step+6)))
					{
						if (std::isdigit(*(step+7)))
						{
							step += 8;
							std::string temp(beg, step);
							temp = " " + temp + " ";
							result.append(temp);
							beg = step;
							afterword = false;
							afterwhitespace = false;
							continue;
						}
						else
						{
							step += 7;
							std::string temp(beg, step);
							temp = " " + temp + " ";
							result.append(temp);
							beg = step;
							afterword = false;
							afterwhitespace = false;
							continue;
						}
					}
					else
					{
						step += 6;
						std::string temp(beg, step);
						temp = " " + temp + "1 ";
						result.append(temp);
						beg = step;
						afterword = false;
						afterwhitespace = false;
						continue;
					}
				}
			}  // END WITHINd

			// now look at words
			else
			{
				// check until end of word
				while (*step != ' ' && !isEndWord(*step) && step != end)
				{
					step++;
				}
				if (afterwhitespace == true && afterword == true && (*step == ' ' || isEndWord(*step) || step == end))
				{
					std::string temp(beg, step);
					temp = " AND " + temp + " ";
					result.append(temp);
					afterwhitespace = false;
					afterword = true;
					beg = step;
					continue;
				}
				else
				{
					std::string temp(beg, step);
					temp = " " + temp + " ";
					result.append(temp);
					afterwhitespace = false;
					afterword = true;
					beg = step;
					continue;
				}
			}  // END words
		}  // END outer while
		// PRINT
		return result;
	}


	std::string normalizeWhitespace(std::string& query)
	{
		std::string result;
		std::string::iterator step = query.begin();
		std::string::iterator beg = step;
		std::string::iterator end = query.end();

		// move step until we find first character
		while (*step == ' ') step++;
		beg = step;

		// now we move through the remaining query
		while (step != end)
		{
			if (*step == ' ')
			{
				do {step++;} while (*step == ' ' && step != end);
				result.append(" ");
				beg = step;
			}
			else
			{
				do {step++;} while (*step != ' ' && step != end);
				std::string temp(beg, step);
				result.append(temp);
				beg = step;
			}
		}

		// move backwards through the result until we hit a character
		std::string::iterator backresult = result.end();
		do {backresult--;} while (*backresult == ' ');
		backresult++;
		result.assign(result.begin(), backresult);

		return result;
	}



	// since setOperators and normalizeWhitespace guarantee a certain structure,
	// we simply have to break the string by whitespace.
	std::vector<std::string> breakLine(std::string& query)
	{
		std::vector<std::string> result;
		std::string::iterator beg = query.begin();
		std::string::iterator step = beg;
		std::string::iterator end = query.end();

		while (step != end)
		{
			if (*step == '"')
			{
				do {step++;} while (*step != '"');
				step++;
				std::string temp(beg, step);
				result.push_back(temp);
				if (*step == ' ') step++;
				beg = step;
			}
			else if (step == (query.end() - 1))
			{
				std::string temp(beg, step + 1);
				result.push_back(temp);
				step++;
				beg = step;
			}
			else if (*step == ' ')
			{
				std::string temp(beg, step);
				result.push_back(temp);
				step++;
				beg = step;
			}
			else
			{
				step++;
			}
		}
		return result;
	}


	/*
	We have two rules for legality:
	1.) Opening and closing parentheses need to be balanced.
		Also we cannot close a parenthesis that hasn't been opened.
		This means the pcounter must be zero after we go through the query_vec.
		And if at any time pcounter is below 0, we return false.
	2.) A word is always followed by an operator,
		an operator is always followed by a word.
		The query always starts with a word and ends with a word.
		We can guarantee all of that using two conditions.
		If at any point opcounter is above 1 or below 0, break and return false.
		If after the loop opcounter is anything but 1, return false.
	*/
	bool isOperator(std::string& term)
	{
		if (term == "AND" || term == "OR" || term == "NOT") return true;
		else if (term.size() >= 5 && term.substr(0,4) == "NEAR" && std::isdigit(term[4])) return true;
		else if (term.size() >= 7 && term.substr(0,6) == "WITHIN" && std::isdigit(term[6])) return true;
		else return false;
	}

	bool testlegality(std::vector<std::string>& query_vec)
	{
		int pcounter = 0;
		int opcounter = 0;

		for (size_t i = 0; i != query_vec.size(); i++)
		{
			if (opcounter > 1 || opcounter < 0) return false;
			else if (pcounter < 0) return false;
			else if (query_vec[i] == "(") pcounter++;
			else if (query_vec[i] == ")") pcounter--;
			else if (preprocessor::isOperator(query_vec[i])) opcounter--;
			else opcounter++;
		}

		if (pcounter != 0 || opcounter != 1) return false;
		else return true;
	}


	std::vector<std::string>::iterator returnWeakestOperator(std::vector<std::string>& query)
	{
		std::vector<std::string>::iterator pos = query.begin();
		unsigned int pcounter = 0;
		while (pos != query.end())
		{
			if (*pos == "(") {pcounter++; pos++; continue;}
			else if (*pos == ")") {pcounter--; pos++; continue;}

			if ((((*pos).substr(0,4) == "NEAR" && std::isdigit((*pos)[4])) ||
				((*pos).substr(0,6) == "WITHIN" && std::isdigit((*pos)[6]))) &&
				pcounter == 1)
			{
				return pos;
			}
			pos++;
		}

		pos = query.begin();
		pcounter = 0;
		while (pos != query.end())
		{
			if (*pos == "(") {pcounter++; pos++; continue;}
			else if (*pos == ")") {pcounter--; pos++; continue;}

			if (*pos == "OR" && pcounter == 1)
			{
				return pos;
			}
			pos++;
		}

		pos = query.begin();
		pcounter = 0;
		while (pos != query.end())
		{
			if (*pos == "(") {pcounter++; pos++; continue;}
			else if (*pos == ")") {pcounter--; pos++; continue;}

			if (*pos == "AND" && pcounter == 1)
			{
				return pos;
			}
			pos++;
		}

		pos = query.begin();
		pcounter = 0;
		while (pos != query.end())
		{
			if (*pos == "(") {pcounter++; pos++; continue;}
			else if (*pos == ")") {pcounter--; pos++; continue;}

			if (*pos == "NOT" && pcounter == 1)
			{
				return pos;
			}
			pos++;
		}

		return pos;
	}


	// for the functionality of setParentheses, query muss pass testlegality()
	std::vector<std::string> setParentheses(std::vector<std::string>& query)
	{
		// if we reach the level of words, we simply return one level up
		std::cout << "Testing leaf level" << std::endl;
		if (query.size() < 2)
		{
			std::cout << "Reached a leaf" << std::endl;
			return query;
		}

		// check number of parentheses set
		std::cout << "Checking parentheses" << std::endl;
		int pcounter = 0;
		bool missing_parentheses = false;
		for (size_t i = 0; i != query.size()-1; i++)
		{
			if (query[i] == "(") {pcounter++;}
			else if (query[i] == ")")
			{
				pcounter--;
				if (pcounter == 0)
				{
					missing_parentheses = true;
					break;
				}
			}
			if (pcounter == 0)
			{
				missing_parentheses = true;
				break;
			}
		}
		if (missing_parentheses == true)
		{
			std::cout << "Entering parentheses" << std::endl;
			query.insert(query.begin(), "(");
			query.push_back("(");
		}

		// find weakest operator at highest level.
		std::cout << "Calling returnWeakestOperator()" << std::endl;
		std::vector<std::string>::iterator pos = preprocessor::returnWeakestOperator(query);
		if (pos == query.end())
		{
			std::cout << "returnWeakestOperator() returned past-the-end iterator" << std::endl;
			std::vector<std::string> empty;
			return empty;
		}

		// recursion
		std::cout << "Left recursion" << std::endl;
		std::vector<std::string> left_st(query.begin() + 1, pos);
		left_st = preprocessor::setParentheses(left_st);

		std::cout << "Right recursion" << std::endl;
		std::vector<std::string> right_st(pos + 1, query.end() - 1);
		right_st = preprocessor::setParentheses(right_st);

		std::cout << "Concatenating vectors" << std::endl;
		std::vector<std::string> result;
		result.push_back("(");
		result.insert(result.end(), left_st.begin(), left_st.end());
		result.push_back(*pos);
		result.insert(result.end(), right_st.begin(), right_st.end());
		result.push_back(")");

		return result;
	}
}