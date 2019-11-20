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
				afterword = true;
				if (afterwhitespace == true && afterword == true)
				{
					std::string temp(beg, step);
					temp = " AND " + temp + " ";
					result.append(temp);
					afterwhitespace = false;
					beg = step;
					continue;
				}
				else
				{
					std::string temp(beg, step);
					temp = " " + temp + " ";
					result.append(temp);
					afterwhitespace = false;
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
				if (afterwhitespace == true && afterword == true && (*step == ' ' || step == end))
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
		std::string::iterator end = query.end();

		while (step != end)
		{
			step++;
		}
	}


	std::string setParentheses(std::string& query)
	{
		std::string result;
		return result;
	}


	std::vector<std::string> breakLine(std::string& query)
	{
		std::vector<std::string> result;
		return result;
	}

	bool testlegality(std::vector<std::string>& query_vec)
	{
		return true;
	}
}