#pragma once

#include <iostream>
#include <string>
#include <algorithm>

namespace fileFunctions
{
	std::string convertToLowerCase(const std::string & str)
	{
		std::string result;
		for (char i : str)
		{
			result += tolower(i);
		}
		return result;
	}

	std::string convertToUpperCase(const std::string & str)
	{
		std::string result;
		for (char i : str)
		{
			result += toupper(i);
		}
		return result;
	}

	std::string removePunctuation(const std::string & str)
	{
		std::string result;
		for (char i : str)
		{
			if (!ispunct(i))
			{
				result += i;
			}
		}
		return result;
	}

	std::string removeSpaces(const std::string & str)
	{
		std::string result;
		for (char i : str)
		{
			if (!isspace(i))
			{
				result += i;
			}
		}
		return result;
	}

	std::string removeLeadingSpaces(const std::string & str)
	{
		for (unsigned int i = 0; i < str.size(); ++i)
		{
			if (!isspace(str.at(i)))
			{
				return std::string(str.cbegin() + i, str.cbegin() + str.size());
			}
		}
		return "";
	}

	std::string removeCharacter(const std::string & str, char ch)
	{
		std::string result;
		for (char i : str)
		{
			if (i != ch)
			{
				result += i;
			}
		}
		return result;
	}

	std::string replaceCharacter(const std::string & str, char remove, char replace)
	{
		std::string result(str.cbegin(), str.cend());
		for (char & i : result)
		{
			if (i == remove)
			{
				i = replace;
			}
		}
		return result;
	}

	std::string invertCase(const std::string & str)
	{
		std::string result;
		for (char i : str)
		{
			if (isalpha(i))
			{
				if (i == tolower(i)) // 'i' is lower-case
				{
					result += toupper(i);
				}
				else // 'i' is upper-case
				{
					result += tolower(i);
				}
			}
			else // 'i' is not a part of the alphabet. Upper/lower-case doesn't make sense for this character
			{
				result += i;
			}
		}
		return result;
	}

	bool startsWithCharacter(const std::string & str, char ch)
	{
		return str.size() > 0 && str.at(0) == ch;
	}

	bool firstNonWhiteSpaceCharacterIs(const std::string & str, char ch)
	{
		for (char i : str)
		{
			if (!isspace(i))
			{
				return i == ch;
			}
		}
		return false;
	}

	bool lengthIs(const std::string & str, std::size_t length)
	{
		return str.size() == length;
	}
}