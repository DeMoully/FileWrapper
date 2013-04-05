#pragma once

#include <iostream>

namespace fileFunctions
{
	namespace FWPF // FileWrapperPrivateFunctions
	{
		void validateBounds(std::size_t & lowerBound, std::size_t & upperBound)
	{
		if (upperBound < lowerBound)
		{
			std::swap(lowerBound, upperBound);
		}
	}
	}
}