#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>
#include <iostream>

#include "FileCloseAction.hpp"
#include "CommonFunctions.hpp"

namespace sp
{
	class FileWrapper final
	{
	public:
		typedef std::vector<std::string>     File;
		typedef File::iterator               Iterator;
		typedef File::const_iterator         ConstIterator;
		typedef File::reverse_iterator       ReverseIterator;
		typedef File::const_reverse_iterator ConstReverseIterator;
	private:
		File            m_contents;
		std::string     m_filename;
		FileCloseAction m_closingAction;
	public:
		// Constructors
		FileWrapper() : m_closingAction(FileCloseAction::NONE)
		{
			// Creates an empty FileWrapper object
		}
		explicit FileWrapper(FileCloseAction closingAction) : m_closingAction(closingAction)
		{
			// Creates an empty FileWrapper object
		}
		explicit FileWrapper(const std::string & filename, FileCloseAction closingAction = FileCloseAction::NONE) : m_filename(filename), m_closingAction(closingAction)
		{
			// Opens and loads a file into memory
			loadFromFile(filename);
		}
		FileWrapper(Iterator first, Iterator last) : m_contents(first, last), m_closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid non-const iterators
		}
		FileWrapper(ConstIterator first, ConstIterator last) : m_contents(first, last), m_closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid const iterators
		}
		FileWrapper(ReverseIterator first, ReverseIterator last) : m_contents(first, last), m_closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid non-const reverse iterators
		}
		FileWrapper(ConstReverseIterator first, ConstReverseIterator last) : m_contents(first, last), m_closingAction(FileCloseAction::NONE)
		{
			// Creates a new FileWrapper object from two valid const reverse iterators
		}
		FileWrapper(const FileWrapper & rhs) : m_contents(rhs.m_contents), m_filename(rhs.m_filename), m_closingAction(rhs.m_closingAction)
		{
			// Copies the contents of one FileWrapper object to another
		}
		FileWrapper(const FileWrapper & rhs, FileCloseAction closingAction) : m_contents(rhs.m_contents), m_filename(rhs.m_filename), m_closingAction(closingAction)
		{
			// Copies the contents of one FileWrapper object to another, but uses a new closing action
		}
		FileWrapper(FileWrapper && rhs) : m_contents(std::move(rhs.m_contents)), m_filename(std::move(rhs.m_filename)), m_closingAction(std::move(rhs.m_closingAction))
		{
			// Move constructor
		}
		// Destructor
		~FileWrapper()
		{
			switch (m_closingAction)
			{
			case FileCloseAction::OUTPUT:
			{
				outputToFile();
				break;
			}
			case FileCloseAction::APPEND:
			{
				appendToFile();
				break;
			}
			}
		}
		// Accessors
		std::string     getFirstLine() const
		{
			// If the file has a first line, returns it. Otherwise returns a blank string.
			return size() ? m_contents.at(0) : "";
		}
		std::string     getLastLine() const
		{
			// If the file has a last line, returns it. Otherwise returns a blank string.
			return size() ? m_contents.at(size() - 1) : "";
		}
		std::string     getLine(std::size_t index) const
		{
			// Returns a line in the file if it exists. Otherwise returns a blank string.
			return index < size() ? m_contents.at(index) : "";
		}
		File            getLines(std::size_t lowerBound, std::size_t upperBound) const
		{
			// Returns a series of lines in the file if they exist. Otherwise returns an empty deque.
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				return File(m_contents.begin() + lowerBound, m_contents.begin() + 1 + std::min(upperBound, size() - 1));
			}
			return File();
		}
		const File &    getContents() const
		{
			// Returns the contents of the file as a deque.
			return m_contents;
		}
		std::string     getFilename() const
		{
			// Returns the name of the file associated with the FileWrapper object
			return m_filename;
		}
		FileCloseAction getClosingAction() const
		{
			// Returns the action that will occur upon destruction
			return m_closingAction;
		}
		std::string     getClosingActionAsString() const
		{
			// Returns the action that will occur upon destruction as a string
			switch (m_closingAction)
			{
			case FileCloseAction::NONE:
			{
				return "NONE";
			}
			case FileCloseAction::OUTPUT:
			{
				return "OUTPUT";
			}
			case FileCloseAction::APPEND:
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
		void setFilename(const std::string & filename)
		{
			// Sets the name of the file associated with the object to the string 'filename'
			// Does not affect any files perviously associated with the FileWrapper object,
			// nor does it affect any of the data held by the FileWrapper object.
			m_filename = filename;
		}
		void setClosingAction(FileCloseAction closingAction)
		{
			// Changes the closing action of the FileWrapper object to the one specified by 'closingAction'
			m_closingAction = closingAction;
		}
		void setLine(std::size_t index, const std::string & str)
		{
			// Sets line[index] to 'str'
			if (index < size())
			{
				m_contents.at(index) = str;
			}
		}
		void appendLine(const std::string & str)
		{
			// Places a line at the end of the file
			m_contents.push_back(str);
		}
		void appendToLine(std::size_t index, const std::string & str)
		{
			// Append the contents of str to the indexth line of the file
			if (index < size())
			{
				m_contents.at(index).append(str);
			}
		}
		void prependLine(const std::string & str)
		{
			// Inserts a line at the beginning of the file
			m_contents.insert(m_contents.begin(), str);
		}
		void prependToLine(std::size_t index, const std::string & str)
		{
			// Prepend the contents of str to the indexth line of the file
			if (index < size())
			{
				m_contents.at(index).insert(m_contents.at(index).begin(), str.begin(), str.end());
			}
		}
		void insertLine(std::size_t index, const std::string & str)
		{
			// Inserts a line before the given index
			if (index < size())
			{
				m_contents.insert(m_contents.begin() + index, str);
			}
		}
		void removeLine(std::size_t index)
		{
			// Removes a line from the file
			if (index < size())
			{
				m_contents.erase(m_contents.begin() + index);
			}
		}
		template <typename FunctionType, typename... Args>
		void removeLineIf(std::size_t index, const FunctionType & function, const Args &... args)
		{
			if (index < size() && function(m_contents.at(index), args...))
			{
				m_contents.erase(m_contents.begin() + index);
			}
		}
		void removeLines(std::size_t lowerBound, std::size_t upperBound)
		{
			// Removes the lines in [lowerBound, upperBound]
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				m_contents.erase(m_contents.begin() + lowerBound, m_contents.begin() + 1 + std::min(upperBound, size() - 1));
			}
		}
		template <typename FunctionType, typename... Args>
		void removeLinesIf(std::size_t lowerBound, std::size_t upperBound, const FunctionType & function, const Args &... args)
		{
			// Goes through each line in [lowerBound, upperBound] and erases it if function(line) == true
			FWPF::validateBounds(lowerBound, upperBound);
			if (lowerBound < size())
			{
				while (lowerBound <= upperBound && lowerBound < size())
				{
					if (function(m_contents.at(lowerBound), args...))
					{
						m_contents.erase(m_contents.begin() + lowerBound);
						if (upperBound != 0) // Need to exit the function now, don't want to wrap around
							--upperBound;
						else
							return;
					}
					else
					{
						++lowerBound;
					}
				}
			}
		}
		void clearContents()
		{
			// Erases every line in the file
			m_contents.erase(m_contents.begin(), m_contents.end());
		}
		template <typename FunctionType, typename... Args>
		void clearContentsIf(const FunctionType & function, const Args &... args)
		{
			// Goes through each line in the file and erases it if function(line) == true
			removeLinesIf(0, size() ? size() - 1 : 0, function, args...);
		}
		// Utilities
		bool        empty() const
		{
			// Returns true if empty, otherwise returns false.
			return m_contents.empty();
		}
		std::size_t size() const
		{
			// Returns the number of lines held by the FileWrapper object
			return m_contents.size();
		}
		std::size_t lineSize(std::size_t index) const
		{
			// Returns the size of a line in the file if the line exists, otherwise returns 0
			return index < size() ? m_contents.at(index).size() : 0;
		}
		void        loadFromFile()
		{
			// Clears the contents of the FileWrapper object, then loads in the
			// data from the file specified by FileWrapper::m_filename
			std::fstream file(m_filename, std::ios::in);
			clearContents();
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					m_contents.push_back(buffer);
				}
			}
		}
		void        loadFromFile(const std::string & filename)
		{
			// Clears the contents of the FileWrapper object, then loads in the
			// data from the file specified by 'filename'
			std::fstream file(filename, std::ios::in);
			clearContents();
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					m_contents.push_back(buffer);
				}
			}
		}
		void        loadFromFileAndAppend()
		{
			// Loads the data from the file specified by FileWrapper::m_filename, then
			// appends it to the data currently held by the FileWrapper object
			std::fstream file(m_filename, std::ios::in);
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					m_contents.push_back(buffer);
				}
			}
		}
		void        loadFromFileAndAppend(const std::string & filename)
		{
			// Loads the data from the file specified by 'filename', then
			// appends it to the data currently held by the FileWrapper
			// object
			std::fstream file(filename, std::ios::in);
			if (file.is_open())
			{
				std::string buffer;
				while (std::getline(file, buffer))
				{
					m_contents.push_back(buffer);
				}
			}
		}
		void		loadFromFileAndPrepend()
		{
			// Loads the data from the file specified by 'm_filename', then
			// prepends it to the data currently held by the FileWrapper
			// object
			std::fstream file(m_filename, std::ios::in);
			if (file.is_open())
			{
				int position = 0;
				std::string buffer;
				while (std::getline(file, buffer))
				{
					m_contents.insert(m_contents.begin() + position++, buffer);
				}
			}
		}
		void		loadFromFileAndPrepend(const std::string & filename)
		{
			// Loads the data from the file specified by 'filename', then
			// prepends it to the data currently held by the FileWrapper
			// object
			std::fstream file(filename, std::ios::in);
			if (file.is_open())
			{
				int position = 0;
				std::string buffer;
				while (std::getline(file, buffer))
				{
					m_contents.insert(m_contents.begin() + position++, buffer);
				}
			}
		}
		void        outputToFile() const
		{
			// Clears the contents of the file specified by FileWrapper::m_filename, then outputs
			// the data held by the FileWrapper object to the file specified by FileWrapper::m_filename
			std::fstream file(m_filename, std::ios::out);
			for (auto i : m_contents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        outputToFile(const std::string & filename) const
		{
			// Clears the contents of the file specified by 'filename', then outputs
			// the data held by the FileWrapper object to the file specified by
			// 'filename'
			std::fstream file(filename, std::ios::out);
			for (auto i : m_contents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        appendToFile() const
		{
			// Appends the contents of the FileWrapper object to
			// the file specified by FileWrapper::m_filename
			std::fstream file(m_filename, std::ios::out | std::ios::app);
			for (auto i : m_contents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        appendToFile(const std::string & filename) const
		{
			// Appends the contents of the FileWrapper object to the file specified
			// by 'filename'. Does not affect the file held by FileWrapper::m_filename
			std::fstream file(filename, std::ios::out | std::ios::app);
			for (auto i : m_contents)
			{
				if (file.is_open())
				{
					file << i << std::endl;
				}
			}
		}
		void        outputToStream(std::ostream & ostr) const
		{
			// Output the contents of the file to a std::ostream (i.e. std::ostream, std::ofstream, etc.) if the stream is valid
			for (auto i : m_contents)
			{
				if (ostr.good())
				{
					ostr << i << std::endl;
				}
			}
		}
		template <typename FunctionType, typename... Args>
		void        applyFunctionToLine(std::size_t index, const FunctionType & function, const Args &... args)
		{
			if (index < size())
			{
				m_contents.at(index) = function(m_contents.at(index), args...);
			}
		}
		template <typename FunctionType, typename... Args>
		void        applyFunctionToLines(std::size_t lowerBound, std::size_t upperBound, const FunctionType & function, const Args &... args)
		{
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				m_contents.at(i) = function(m_contents.at(i), args...);
			}
		}
		template <typename FunctionType, typename... Args>
		void        applyFunctionToContents(const FunctionType & function, const Args &... args)
		{
			for (auto & i : m_contents)
			{
				i = function(i, args...);
			}
		}
		void        mergeAndAppend(const FileWrapper & rhs)
		{
			// Adds the contents of rhs to the end of the FileWrapper object
			for (unsigned int i = 0; i < rhs.size(); ++i)
			{
				appendLine(rhs.getLine(i));
			}
		}
		void        mergeAndAppend(Iterator begin, Iterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndAppend(ConstIterator begin, ConstIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndAppend(ReverseIterator begin, ReverseIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndAppend(ConstReverseIterator begin, ConstReverseIterator end)
		{
			// Adds the contents of [begin, end) to the end of the FileWrapper object
			while (begin != end)
			{
				appendLine(*begin++);
			}
		}
		void        mergeAndPrepend(const FileWrapper & rhs)
		{
			// Prepends the contents of rhs to the FileWrapper object
			std::size_t position = 0;
			for (unsigned int i = 0; i < rhs.size(); ++i)
			{
				insertLine(position++, rhs.getLine(i));
			}
		}
		void        mergeAndPrepend(Iterator begin, Iterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndPrepend(ConstIterator begin, ConstIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndPrepend(ReverseIterator begin, ReverseIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndPrepend(ConstReverseIterator begin, ConstReverseIterator end)
		{
			// Prepends the contents of [begin, end) to the FileWrapper object
			std::size_t position = 0;
			while (begin != end)
			{
				insertLine(position++, *begin++);
			}
		}
		void        mergeAndInsert(std::size_t index, const FileWrapper & rhs)
		{
			// Merge the contents of rhs with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() -1]
			if (index < size())
			{
				for (unsigned int i = 0; i < rhs.size(); ++i)
				{
					insertLine(index++, rhs.getLine(i));
				}
			}
		}
		void        mergeAndInsert(std::size_t index, Iterator begin, Iterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		void        mergeAndInsert(std::size_t index, ConstIterator begin, ConstIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		void        mergeAndInsert(std::size_t index, ReverseIterator begin, ReverseIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		void        mergeAndInsert(std::size_t index, ConstReverseIterator begin, ConstReverseIterator end)
		{
			// Merge the contents of [begin, end) with those of the FileWrapper object, starting at index
			// Each line is inserted before index, so the valid range is [0, size() - 1]
			if (index < size())
			{
				while (begin != end)
				{
					insertLine(index++, *begin++);
				}
			}
		}
		// Iterators
		Iterator             begin()
		{
			// Return an iterator to the beginning of the file
			return m_contents.begin();
		}
		Iterator             end()
		{
			// Return an iterator to the end of the file
			return m_contents.end();
		}
		ConstIterator        cbegin() const
		{
			// Return a const iterator to the beginning of the file
			return m_contents.cbegin();
		}
		ConstIterator        cend() const
		{
			// Return a const iterator to the end of the file
			return m_contents.cend();
		}
		ReverseIterator      rbegin()
		{
			// Return a reverse iterator to the (reverse) beginning of the file
			return m_contents.rbegin();
		}
		ReverseIterator      rend()
		{
			// Return a reverse iterator to the (reverse) end of the file
			return m_contents.rend();
		}
		ConstReverseIterator crbegin() const
		{
			// Return a const reverse iterator to the (reverse) beginning of the file
			return m_contents.crbegin();
		}
		ConstReverseIterator crend() const
		{
			// Return a const reverse iterator to the (reverse) end of the file
			return m_contents.crend();
		}
		Iterator             find(char character)
		{
			// Find the first line containing character and return an iterator to that line
			Iterator iterator = begin();
			while (iterator != end())
			{
				std::string::size_type position = iterator->find(character);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		Iterator             find(const std::string & str)
		{
			// Find the first line containing str and return an iterator to that line
			Iterator iterator = begin();
			while (iterator != end())
			{
				std::string::size_type position = iterator->find(str);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		ConstIterator        find(char character) const
		{
			// Find the first line containing character and return an iterator to that line
			ConstIterator iterator = cbegin();
			while (iterator != cend())
			{
				std::string::size_type position = iterator->find(character);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		ConstIterator        find(const std::string & str) const
		{
			// Find the first line containing str and return an iterator to that line
			ConstIterator iterator = cbegin();
			while (iterator != cend())
			{
				std::string::size_type position = iterator->find(str);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		ReverseIterator      rfind(char character)
		{
			// Find the first line containing character and return an iterator to that line
			ReverseIterator iterator = rbegin();
			while (iterator != rend())
			{
				std::string::size_type position = iterator->rfind(character);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		ReverseIterator      rfind(const std::string & str)
		{
			// Find the first line containing str and return an iterator to that line
			ReverseIterator iterator = rbegin();
			while (iterator != rend())
			{
				std::string::size_type position = iterator->rfind(str);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		ConstReverseIterator rfind(char character) const
		{
			// Find the first line containing character and return an iterator to that line
			ConstReverseIterator iterator = crbegin();
			while (iterator != crend())
			{
				std::string::size_type position = iterator->rfind(character);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		ConstReverseIterator rfind(const std::string & str) const
		{
			// Find the first line containing str and return an iterator to that line
			ConstReverseIterator iterator = crbegin();
			while (iterator != crend())
			{
				std::string::size_type position = iterator->rfind(str);
				if (position != std::string::npos)
				{
					return iterator;
				}
				++iterator;
			}
			return iterator;
		}
		// Overloaded Operators
		FileWrapper &       operator =  (const FileWrapper & rhs)
		{
			// Copy assignment operator
			m_contents = rhs.getContents();
			m_filename = rhs.getFilename();
			m_closingAction = rhs.getClosingAction();
			return *this;
		}
		FileWrapper &       operator =  (FileWrapper && rhs)
		{
			// Move assignment operator
			m_contents = std::move(rhs.getContents());
			m_filename = std::move(rhs.getFilename());
			m_closingAction = std::move(rhs.getClosingAction());
			return *this;
		}
		bool                operator == (const FileWrapper & rhs) const
		{
			// Returns true if all the components are equal, otherwise returns false
			return m_contents == rhs.getContents() && m_filename == rhs.getFilename() && m_closingAction == rhs.getClosingAction();
		}
		bool                operator != (const FileWrapper & rhs) const
		{
			// Returns true if any of the components are not equal, otherwise returns false
			return m_contents != rhs.getContents() || m_filename != rhs.getFilename() || m_closingAction != rhs.getClosingAction();
		}
		const std::string & operator [] (std::size_t index) const
		{
			// Doesn't perform any bounds checking, leaves that to the container
			return m_contents.at(index);
		}
		std::string &       operator [] (std::size_t index)
		{
			// Doesn't perform any bounds checking, leaves that to the container
			return m_contents.at(index);
		}
	};
}