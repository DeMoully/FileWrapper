#pragma once

#include <ostream>

namespace fileFunctions
{
	enum class FileCloseAction
	{
		NONE, // Perform no actions upon deletion of object
		OUTPUT, // Output the current contents of the object to the file specified by fileName
		APPEND // Append the current contents of the object to the file specified by fileName
	};

	std::ostream & operator << (std::ostream & ostr, FileCloseAction rhs)
	{
		switch (rhs)
		{
		case FileCloseAction::NONE:
			{
				ostr << "NONE";
				break;
			}
		case FileCloseAction::OUTPUT:
			{
				ostr << "OUTPUT";
				break;
			}
		case FileCloseAction::APPEND:
			{
				ostr << "APPEND";
				break;
			}
		}
		return ostr;
	}
}