// FileWrapper Class

#pragma once

#include <fstream>
#include <deque>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

namespace fileFunctions
{
	typedef std::deque<std::string>::iterator               FileIterator;
	typedef std::deque<std::string>::const_iterator         ConstFileIterator;
	typedef std::deque<std::string>::reverse_iterator       ReverseFileIterator;
	typedef std::deque<std::string>::const_reverse_iterator ConstReverseFileIterator;

	enum FileCloseAction
	{
		NONE, // Perform no actions upon deletion of object
		OUTPUT, // Output the current contents of the object to the file specified by fileName
		APPEND // Append the current contents of the object to the file specified by fileName
	};

	class FileWrapper
	{
	private:
		std::deque<std::string> fileContents;
		std::string             fileName;
		FileCloseAction         closingAction;
	public:
		// Constructors
		FileWrapper         () : closingAction(NONE)
		{
			// Creates an empty FileWrapper object
		}
		explicit FileWrapper(FileCloseAction onClose) : closingAction(onClose)
		{
			// Creates an empty FileWrapper object
		}
		explicit FileWrapper(const std::string & filePath, FileCloseAction onClose = NONE) : fileName(filePath), closingAction(onClose)
		{
			// Opens and loads a file into memory
			loadFromFile(filePath);
		}
		FileWrapper         (FileIterator first, FileIterator last) : fileContents(first, last)
		{
			// Creates a new FileWrapper object from two valid non-const iterators
		}
		FileWrapper         (ConstFileIterator first, ConstFileIterator last) : fileContents(first, last)
		{
			// Creates a new FileWrapper object from two valid const iterators
		}
		FileWrapper         (ReverseFileIterator first, ReverseFileIterator last) : fileContents(first, last)
		{
			// Creates a new FileWrapper object from two valid non-const reverse iterators
		}
		FileWrapper         (ConstReverseFileIterator first, ConstReverseFileIterator last) : fileContents(first, last)
		{
			// Creates a new FileWrapper object from two valid const reverse iterators
		}
		FileWrapper         (const FileWrapper & rhs) : fileContents(rhs.fileContents), fileName(rhs.fileName), closingAction(rhs.closingAction)
		{
			// Copies the contents of one FileWrapper object to another
		}
		FileWrapper         (FileWrapper && rhs) : fileContents(std::move(rhs.fileContents)), fileName(std::move(rhs.fileName)), closingAction(std::move(rhs.closingAction))
		{
			// Move constructor
		}
		// Destructor
		~FileWrapper()
		{
			switch (closingAction)
			{
			case OUTPUT:
				{
					outputToFile();
					break;
				}
			case APPEND:
				{
					appendToFile();
					break;
				}
			}
		}
		// Accessors
		std::string                     getFirstLine            () const
		{
			// If the file has a first line, returns it. Otherwise returns a blank string.
			return fileContents.size() ? fileContents.at(0) : "";
		}
		std::string                     getLastLine             () const
		{
			// If the file has a last line, returns it. Otherwise returns a blank string.
			return fileContents.size() ? fileContents.at(fileContents.size() - 1) : "";
		}
		std::string                     getLine                 (std::size_t index) const
		{
			// Returns a line in the file if it exists. Otherwise returns a blank string.
			return index < fileContents.size() ? fileContents.at(index) : ""; 
		}
		std::deque<std::string>         getLines                (std::size_t lowerBound, std::size_t upperBound) const
		{
			// Returns a series of lines in the file if they exist. Otherwise returns an empty deque.
			if (lowerBound < fileContents.size() || upperBound < fileContents.size())
			{
				return std::deque<std::string>(fileContents.cbegin() + std::min(lowerBound, upperBound), fileContents.cbegin() + 1 + std::min(std::max(lowerBound, upperBound), fileContents.size() - 1));
			}
			return std::deque<std::string>();
		}
		const std::deque<std::string> & getFileContents         () const
		{
			// Returns the contents of the file as a deque.
			return fileContents;
		}
		std::string                     getFileName             () const
		{
			// Returns the name of the file associated with the FileWrapper object
			return fileName;
		}
		FileCloseAction                 getClosingAction        () const
		{
			// Returns the action that will occur upon destruction
			return closingAction;
		}
		std::string                     getClosingActionAsString() const
		{
			// Returns the action that will occur upon destruction as a string
			switch (closingAction)
			{
			case NONE:
				{
					return "NONE";
				}
			case OUTPUT:
				{
					return "OUTPUT";
				}
			case APPEND:
				{
					return "APPEND";
				}
			default:
				{
					return "NONE";
				}
			}
		}
		// Mutators
		void setFileName     (const std::string & filePath)
		{
			// Sets the name of the file associated with the object to the string 'filePath'
			// Does not affect any files perviously associated with the FileWrapper object,
			// nor does it affect any of the data held by the FileWrapper object.
			fileName = filePath;
		}
		void setClosingAction(FileCloseAction onClose)
		{
			// Changes the closing action of the FileWrapper object to the one specified by 'onClose'
			closingAction = onClose;
		}
		void setLine         (std::size_t index, const std::string & str)
		{
			// Sets line[index] to 'str'
			if (index < fileContents.size())
			{
				fileContents.at(index) = str;
			}
		}
		void appendLine      (const std::string & str)
		{
			// Places a line at the end of the file
			fileContents.push_back(str);
		}
		void prependLine     (const std::string & str)
		{
			// Inserts a line at the beginning of the file
			fileContents.push_front(str);
		}
		void insertLine      (std::size_t index, const std::string & str)
		{
			// Inserts a line before the given index
			if (index < fileContents.size())
			{
				fileContents.insert(fileContents.begin() + index, str);
			}
		}
		void removeLine      (std::size_t index)
		{
			// Removes a line from the file
			if (index < fileContents.size())
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &)> & function)
		{
			// Removes a line if function(line) == true
			if (index < fileContents.size() && function(fileContents.at(index)))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		template <class T>
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &, const T &)> & function, const T & parameter)
		{
			// Removes a line if function(line, parameter) == true
			if (index < fileContents.size() && function(fileContents.at(index), parameter))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		template <class T, class U>
		void removeLineIf    (std::size_t index, const std::function<bool (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Removes a line if function(line, parameterOne, parameterTwo) == true
			if (index < fileContents.size() && function(fileContents.at(index), parameterOne, parameterTwo))
			{
				fileContents.erase(fileContents.begin() + index);
			}
		}
		void removeLines     (std::size_t lowerBound, std::size_t upperBound)
		{
			// Removes the lines in [lowerBound, upperBound]
			if (std::min(lowerBound, upperBound) < fileContents.size())
			{
				fileContents.erase(fileContents.begin() + std::min(lowerBound, upperBound), fileContents.begin() + 1 + std::min(std::max(lowerBound, upperBound), fileContents.size() - 1));
			}
		}
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &)> & function)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line) == true
			if (lowerBound < fileContents.size() || upperBound < fileContents.size())
			{
				if (lowerBound > upperBound)
				{
					std::swap(lowerBound, upperBound);
				}
				while (lowerBound <= upperBound && lowerBound < fileContents.size())
				{
					if (function(fileContents.at(lowerBound)))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		template <class T>
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &, const T &)> & function, const T & parameter)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line, parameter) == true
			if (lowerBound < fileContents.size() || upperBound < fileContents.size())
			{
				if (lowerBound > upperBound)
				{
					std::swap(lowerBound, upperBound);
				}
				while (lowerBound <= upperBound && lowerBound < fileContents.size())
				{
					if (function(fileContents.at(lowerBound), parameter))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		template <class T, class U>
		void removeLinesIf   (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line, parameterOne, parameterTwo) == true
			if (lowerBound < fileContents.size() || upperBound < fileContents.size())
			{
				if (lowerBound > upperBound)
				{
					std::swap(lowerBound, upperBound);
				}
				while (lowerBound <= upperBound && lowerBound < fileContents.size())
				{
					if (function(fileContents.at(lowerBound), parameterOne, parameterTwo))
					{
						fileContents.erase(fileContents.begin() + lowerBound);
						--upperBound;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		void clearContents   ()
		{
			// Erases every line in the file
			fileContents.erase(fileContents.begin(), fileContents.end());
		}
		void clearContentsIf (const std::function<bool (const std::string &)> & function)
		{
			// Goes through each line in the file and erases it if function(line) == true
			std::size_t begin = 0;
			std::size_t end = fileContents.size();
			while (begin != end)
			{
				if (function(fileContents.at(begin)))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		template <class T>
		void clearContentsIf (const std::function<bool (const std::string &, const T &)> & function, const T & parameter)
		{
			// Goes through each line in the file and erases it if function(line, parameter) == true
			std::size_t begin = 0;
			std::size_t end = fileContents.size();
			while (begin != end)
			{
				if (function(fileContents.at(begin), parameter))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		template <class T, class U>
		void clearContentsIf (const std::function<bool (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Goes through each line in the file and erases it if function(line, parameterOne, parameterTwo) == true
			std::size_t begin = 0;
			std::size_t end = fileContents.size();
			while (begin != end)
			{
				if (function(fileContents.at(begin), parameterOne, parameterTwo))
				{
					fileContents.erase(fileContents.begin() + begin);
					--end;
				}
				else
				{
					++begin;
				}
			}
		}
		// Utilities
		bool        empty                  () const
		{
			// Returns true if empty, otherwise returns false.
			return fileContents.empty();
		}
		std::size_t size                   () const
		{
			// Returns the number of lines held by the FileWrapper object
			return fileContents.size();
		}
		void        loadFromFile           ()
		{
			// Clears the contents of the FileWrapper object, then loads in the
			// data from the file specified by FileWrapper::fileName
			std::fstream file(fileName, std::ios::in);
			clearContents();
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        loadFromFile           (const std::string & filePath)
		{
			// Clears the contents of the FileWrapper object, then loads in the
			// data from the file specified by 'filePath'
			std::fstream file(filePath, std::ios::in);
			clearContents();
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        loadFromFileAndAppend  ()
		{
			// Loads the data from the file specified by FileWrapper::fileName, then
			// appends it to the data currently held by the FileWrapper object
			std::fstream file(fileName, std::ios::in);
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        loadFromFileAndAppend  (const std::string & filePath)
		{
			// Loads the data from the file specified by 'filePath', then
			// appends it to the data currently held by the FileWrapper
			// object
			std::fstream file(filePath, std::ios::in);
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					fileContents.push_back(buffer);
				}
			}
		}
		void        outputToFile           () const
		{
			// Clears the contents of the file specified by FileWrapper::fileName, then outputs
			// the data held by the FileWrapper object to the file specified by FileWrapper::fileName
			std::fstream file(fileName, std::ios::out);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        outputToFile           (const std::string & filePath) const
		{
			// Clears the contents of the file specified by 'filePath', then outputs
			// the data held by the FileWrapper object to the file specified by
			// 'filePath'
			std::fstream file(filePath, std::ios::out);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        appendToFile           () const
		{
			// Appends the contents of the FileWrapper object to
			// the file specified by FileWrapper::fileName
			std::fstream file(fileName, std::ios::out | std::ios::app);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        appendToFile           (const std::string & filePath) const
		{
			// Appends the contents of the FileWrapper object to the file specified
			// by 'filePath'. Does not affect the file held by FileWrapper::fileName
			std::fstream file(filePath, std::ios::out | std::ios::app);
			for (auto i : fileContents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        outputToStream         (std::ostream & ostr) const
		{
			// Output the contents of the file to a std::ostream (i.e. std::ostream, std::ofstream, etc.) if the stream is valid
			for (auto i : fileContents)
			{
				if (ostr.good())
				{
					ostr << i << std::endl;
				}
			}
		}
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &)> & function)
		{
			// Apply a function that takes a single string as an argument to a single line in the file
			if (index < fileContents.size())
			{
				fileContents.at(index) = function(fileContents.at(index));
			}
		}
		template <class T>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &)> & function, const T & parameter)
		{
			// Apply a function that takes a single string and another parameter as arguments to a single line in the file
			if (index < fileContents.size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameter);
			}
		}
		template <class T, class U>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Apply a function that takes a single string and two other parameters as arguments to a single line of the file
			if (index < fileContents.size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameterOne, parameterTwo);
			}
		}
		template <class T, class U, class V>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Apply a function that takes a single string and three other parameters as arguments to a single line in the file
			if (index < fileContents.size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameterOne, parameterTwo, parameterThree);
			}
		}
		template <class T, class U, class V, class W>
		void        applyFunctionToLine    (std::size_t index, const std::function<std::string (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Apply a function that takes a single string and four other parameters as arguments to a single line in the file
			if (index < fileContents.size())
			{
				fileContents.at(index) = function(fileContents.at(index), parameterOne, parameterTwo, parameterThree, parameterFour);
			}
		}
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &)> & function)
		{
			// Apply a function that takes a single string as an argument to a series of lines in the file
			for (unsigned int i = std::min(lowerBound, upperBound); i <= std::max(lowerBound, upperBound) && i < fileContents.size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i));
			}
		}
		template <class T>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &)> & function, const T & parameter)
		{
			// Apply a function that takes a single string an another parameter as arguments to a series of lines in the file
			for (unsigned int i = std::mind(lowerBound, upperBound); i <= std::max(lowerBound, upperBound) && i < fileContents.size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameter);
			}
		}
		template <class T, class U>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Apply a function that takes a single string and two other parameters as arugments to a series of lines in the file
			for (unsigned int i = std::min(lowerBound, upperBound); i <= std::max(lowerBound, upperBound) && i < fileContents.size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameterOne, parameterTwo);
			}
		}
		template <class T, class U, class V>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Apply a function that takes a single string and three other parameters as arguments to a series of lines in the file
			for (unsigned int i = std::min(lowerBound, upperBound); i <= std::max(lowerBound, upperBound) && i < fileContents.size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameterOne, parameterTwo, parameterThree);
			}
		}
		template <class T, class U, class V, class W>
		void        applyFunctionToLines   (std::size_t lowerBound, std::size_t upperBound, const std::function<std::string (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Apply a function that takes a single string and four other parameters as arguments to a series of lines in the file
			for (unsigned int i = std::min(lowerBound, upperBound); i <= std::max(lowerBound, upperBound) && i < fileContents.size(); ++i)
			{
				fileContents.at(i) = function(fileContents.at(i), parameterOne, parameterTwo, parameterThree, parameterFour);
			}
		}
		void        applyFunctionToContents(const std::function<std::string (const std::string &)> & function)
		{
			// Apply a function that takes a single string as an argument to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i);
			}
		}
		template <class T>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &)> & function, const T & parameter)
		{
			// Apply a function that takes a single string and another parameter as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameter);
			}
		}
		template <class T, class U>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &, const U &)> & function, const T & parameterOne, const U & parameterTwo)
		{
			// Apply a function that takes a single string and two other parameters as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameterOne, parameterTwo);
			}
		}
		template <class T, class U, class V>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &, const U &, const V &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree)
		{
			// Apply a function that takes a single string and three other parameters as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameterOne, parameterTwo, parameterThree);
			}
		}
		template <class T, class U, class V, class W>
		void        applyFunctionToContents(const std::function<std::string (const std::string &, const T &, const U &, const V &, const W &)> & function, const T & parameterOne, const U & parameterTwo, const V & parameterThree, const W & parameterFour)
		{
			// Apply a function that takes a single string and four other parameters as arguments to each line in the file
			for (auto & i : fileContents)
			{
				i = function(i, parameterOne, parameterTwo, parameterThree, parameterFour);
			}
		}
		// Iterators
		FileIterator             begin  ()
		{
			return fileContents.begin();
		}
		FileIterator             end    ()
		{
			return fileContents.end();
		}
		ConstFileIterator        cbegin () const
		{
			return fileContents.cbegin();
		}
		ConstFileIterator        cend   () const
		{
			return fileContents.cend();
		}
		ReverseFileIterator      rbegin ()
		{
			return fileContents.rbegin();
		}
		ReverseFileIterator      rend   ()
		{
			return fileContents.rend();
		}
		ConstReverseFileIterator crbegin() const
		{
			return fileContents.crbegin();
		}
		ConstReverseFileIterator crend  () const
		{
			return fileContents.crend();
		}
		// Overloaded Operators
		FileWrapper &       operator =  (const FileWrapper & rhs)
		{
			// Copy assignment operator
			fileContents = rhs.getFileContents();
			fileName = rhs.getFileName();
			closingAction = rhs.getClosingAction();
			return *this;
		}
		FileWrapper &       operator =  (FileWrapper && rhs)
		{
			// Move assignment operator
			fileContents = std::move(rhs.getFileContents());
			fileName = std::move(rhs.getFileName());
			closingAction = std::move(rhs.getClosingAction());
			return *this;
		}
		bool                operator == (const FileWrapper & rhs) const
		{
			// Returns true if all the components are equal, otherwise returns false
			return fileContents == rhs.getFileContents() && fileName == rhs.getFileName() && closingAction == rhs.getClosingAction();
		}
		bool                operator != (const FileWrapper & rhs) const
		{
			// Returns true if any of the components are not equal, otherwise returns false
			return fileContents != rhs.getFileContents() || fileName != rhs.getFileName() || closingAction != rhs.getClosingAction();
		}
		const std::string & operator [] (std::size_t index) const
		{
			// Doesn't perform any bounds checking, leaves that to the deque
			return fileContents.at(index); 
		}
		std::string &       operator [] (std::size_t index)
		{
			// Doesn't perform any bounds checking, leaves that to the deque
			return fileContents.at(index);
		}
	};
};