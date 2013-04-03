#pragma once

namespace fileFunctions
{
	enum class FileCloseAction
	{
		NONE, // Perform no actions upon deletion of object
		OUTPUT, // Output the current contents of the object to the file specified by fileName
		APPEND // Append the current contents of the object to the file specified by fileName
	};
}