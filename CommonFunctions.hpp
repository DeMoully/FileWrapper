#pragma once

#include <iostream>
#include <string>
#include <cstdio>

namespace fileFunctions
{
	namespace FWPF // FileWrapperPrivateFunctions
	{
		// If lowerBound >= upperBound, swap lowerBound and upperBound
		void validateBounds(std::size_t & lowerBound, std::size_t & upperBound)
		{
			if (upperBound < lowerBound)
			{
				std::swap(lowerBound, upperBound);
			}
		}

		// Rename the file 'oldName' to 'newName' if it exists
		bool renameFile(const std::string & oldName, const std::string & newName)
		{
			return !std::rename(oldName.c_str(), newName.c_str());
		}

		// Removes the file 'fileName' if possible
		bool removeFile(const std::string & fileName)
		{
			return !std::remove(fileName.c_str());
		}
	}
}