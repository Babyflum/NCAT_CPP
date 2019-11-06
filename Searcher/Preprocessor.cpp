// Copyright 2019, Uni Freiburg
// Fatlum Sadiku, <fatlumsadiku91@gmail.com>


// standard header includes
#include <string>
#include <vector>
#include <cctype>


namespace preprocessor 
{
	std::string setOperators(std::string& query)
	{
		std::string result;
		std::string::iterator step = query.begin(); // this iterator marks the character we are currently at
		std::string::iterator beg = step;  // this iterator marks the beginning of a word
		std::string::iterator end = query.end();
		bool afterword = false;
		bool afterwhitespace = false;

		while (step != end)
		{
			// we'll start by looking at the vanilla variations
			if (*step == '&') result.append(" AND "); step++; afterword = false; continue;
			else if (*step == '|') result.append(" OR "); step++; afterword = false; continue;
			else if (*step == '~') result.append(" NOT "); step++; afterword = false; continue;
			else if (*step == '(') result.append(" ( "); step++; afterword = false; continue;
			else if (*step == ')') result.append(" ) "); step++; afterword = false; continue;

			// now we look at whitespaces and commas
			else if ((*step == ' ') && (afterword == true)) afterwhitespace = true; step++; continue; 
			else if ((*step == ',') && (afterword == true)) result.append(" OR "); step++; afterword = false; continue;

			// now we look at NEAR
			else if (*step == 'N')
			{
				beg = step++;
				if (std::isdigit(*step)) 
				{
					while (std::isdigit(*step)) {step++;}
					std::string temp(beg, step);
					temp = " NEAR" + temp + " ";
					result.append(temp);
					afterword = false;
					afterwhitespace = false;
					continue;
				}
				else if ((*step == 'E') && (*(step + 1) == 'A') && (*(step + 2) == 'R'))
				{
					step = step + 3;
					// if no number follows NEAR, use some default; or throw exception. I don't know yet.
					if (!std::isdigit(*step)) result.append(" NEAR1 "); step++; afterword = false; continue;
					else
					{
						beg = step;
						while (std::isdigit(*step)) {step++;}
						std::string temp(beg, step);
						temp = " NEAR" + temp + " ";
						result.append(temp);
						afterword = false;
						afterwhitespace = false;
						continue;
					}
				}
			}

			// now we look at WITHIN
			else if (*step == 'W')
			{
				beg = step++;
				if (std::isdigit(*step)) 
				{
					while (std::isdigit(*step)) {step++;}
					std::string temp(beg, step);
					temp = " WITHIN" + temp + " ";
					result.append(temp);
					afterword = false;
					afterwhitespace = false;
					continue;
				}
				else if ((*step == 'I') && (*(step + 1) == 'T') && (*(step + 2) == 'H') && (*(step + 3) == 'I') && (*(step + 4) == 'N'))
				{
					step = step + 4;
					// if no number follows NEAR, use some default; or throw exception. I don't know yet.
					if (!std::isdigit(*step)) result.append(" WITHIN1 "); step++; afterword = false; continue;
					else
					{
						beg = step;
						while (std::isdigit(*step)) {step++;}
						std::string temp(beg, step);
						temp = " WITHIN" + temp + " ";
						result.append(temp);
						afterword = false;
						afterwhitespace = false;
						continue;
					}
				}
			}

			// if whitespace doesn't come after word, just pass
			else if ((*step == ' ') && (afterword == false)) step++; continue;

			// if we encounter a character after a whitespace and after a word, it's an AND
			else if (std::isalpha(*step) && (afterword == true) && (afterwhitespace == true))
			{
				result.append(" AND ");
				afterword = false;
				afterwhitespace = false;
				beg = step;
				continue;
			}

			// finally we check for normal words.
			else if (std::isalpha(*step) && (afterwhitespace == false))
			{
				beg = step;
				while (!std::isspace(*step) && !(*step == '(') && !(*step == ')')) step++;
				std::string temp(beg, step);
				temp = " " + temp + " ";
				result.append(temp);
				afterword = true;
				continue;
			}
			else if (*step == '(') result.append(" ( "); step++; afterword = false; continue;
			else if (*step == ')') result.append(" ) "); step++; afterword = false; continue;
		}
	}


	std::string normalizeWhitespace(std::string& query)
	{
		;
	}


	std::string setParentheses(std::string& query)
	{
		;
	}


	std::vector<std::string> breakLine(std::string& query)
	{
		;
	}

	bool testlegality(std::vector<std::string>& query_vec)
	{
		;
	}
}