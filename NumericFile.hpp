#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <algorithm>
#include <functional>
#include <string>

#include "FileCloseAction.hpp"

namespace fileFunctions
{
	typedef std::deque<double> NumericLine;

	typedef std::deque<NumericLine>::iterator               NumericFileIterator;
	typedef std::deque<NumericLine>::const_iterator         ConstNumericFileIterator;
	typedef std::deque<NumericLine>::reverse_iterator       ReverseNumericFileIterator;
	typedef std::deque<NumericLine>::const_reverse_iterator ConstReverseNumericFileIterator;

	typedef NumericLine::iterator               NumericLineIterator;
	typedef NumericLine::const_iterator         ConstNumericLineIterator;
	typedef NumericLine::reverse_iterator       ReverseNumericLineIterator;
	typedef NumericLine::const_reverse_iterator ConstReverseNumericLineIterator;

	class NumericFile
	{
	private:
		std::deque<NumericLine> contents;
		std::string fileName;
		FileCloseAction closingAction;
	public:
		// Constructors
		NumericFile         () : closingAction(FileCloseAction::NONE)
		{
			// Create an empty NumericFile object
		}
		explicit NumericFile(FileCloseAction onClose) : closingAction(onClose)
		{
			// Create a NumericFile object that is not associated with any files and does not load any data upon creation
		}
		explicit NumericFile(const std::string filePath, FileCloseAction onClose = FileCloseAction::NONE) : fileName(filePath), closingAction(onClose)
		{
			// Creates a NumericFile object that is associated with a file and loads data upon creation
			loadFromFile(filePath);
		}
		NumericFile         (NumericFileIterator first, NumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (ConstNumericFileIterator first, ConstNumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (ReverseNumericFileIterator first, ReverseNumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (ConstReverseNumericFileIterator first, ConstReverseNumericFileIterator last) : contents(first, last), closingAction(FileCloseAction::NONE)
		{
			// Creates a NumericFile from an iterator range
		}
		NumericFile         (const NumericFile & rhs) : contents(rhs.contents), fileName(rhs.fileName), closingAction(rhs.closingAction)
		{
			// Copy constructor
		}
		NumericFile         (NumericFile && rhs) : contents(std::move(rhs.contents)), fileName(std::move(rhs.fileName)), closingAction(std::move(rhs.closingAction))
		{
			// Move constructor
		}
		// Destructor
		~NumericFile()
		{
			switch (closingAction)
			{
			case FileCloseAction::OUTPUT: // Output the contents to 'fileName'
				{
					outputToFile();
					break;
				}
			case FileCloseAction::APPEND: // Append the contents to 'fileName'
				{
					appendToFile();
					break;
				}
			}
		}
		// Accessors
		double                                 getEntry                (std::size_t line, std::size_t index) const
		{
			return (line < contents.size() && index < contents.at(line).size()) ? contents.at(line).at(index) : 0;
		}
		NumericLine                            getLine                 (std::size_t line) const
		{
			return (line < contents.size()) ? contents.at(line) : NumericLine();
		}
		const std::deque<NumericLine> &        getFileContents         () const
		{
			return contents;
		}
		std::string                            getFileName             () const
		{
			return fileName;
		}
		FileCloseAction                        getClosingAction        () const
		{
			return closingAction;
		}
		std::string                            getClosingActionAsString() const
		{
			switch (closingAction)
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
		void setFileName       (const std::string & filePath)
		{
			fileName = filePath;
		}
		void setClosingAction  (FileCloseAction onClose)
		{
			closingAction = onClose;
		}
		void setEntry          (std::size_t line, std::size_t index, double value)
		{
			if (line < contents.size() && index < contents.at(line).size())
			{
				contents.at(line).at(index) = value;
			}
		}
		void appendEntryToLine (std::size_t line, double value)
		{
			if (line < contents.size())
			{
				contents.at(line).push_back(value);
			}
		}
		void prependEntryToLine(std::size_t line, double value)
		{
			if (line < contents.size())
			{
				contents.at(line).push_front(value);
			}
		}
		void insertEntryInLine (std::size_t line, std::size_t index, double value)
		{
			if (line < contents.size() && index < contents.at(line).size())
			{
				contents.at(line).insert(contents.at(line).begin() + index, value);
			}
		}
		void appendLineToFile  (NumericLineIterator first, NumericLineIterator last)
		{
			contents.emplace_back(first, last);
		}
		void prependLineToFile (NumericLineIterator first, NumericLineIterator last)
		{
			contents.emplace_front(first, last);
		}
		void insertLineInFile  (std::size_t line, NumericLineIterator first, NumericLineIterator last)
		{
			if (line < contents.size())
			{
				contents.emplace(contents.begin() + line, first, last);
			}
		}
		void appendLineToFile  (const NumericLine & line)
		{
			contents.push_back(line);
		}
		void prependLineToFile (const NumericLine & line)
		{
			contents.push_front(line);
		}
		void insertLineInFile  (std::size_t line, const NumericLine & numericLine)
		{
			if (line < contents.size())
			{
				contents.insert(contents.begin() + line, numericLine);
			}
		}
		void removeEntry       (std::size_t line, std::size_t index)
		{
			if (line < contents.size() && index < contents.at(line).size())
			{
				contents.at(line).erase(contents.at(line).begin() + index);
			}
		}
		void removeLine        (std::size_t line)
		{
			if (line < contents.size())
			{
				contents.erase(contents.begin() + line);
			}
		}
		void clearContents     ()
		{
			contents.erase(contents.begin(), contents.end());
		}
		// Utilities
		bool        empty                  () const
		{
			return contents.empty();
		}
		std::size_t size                   () const
		{
			return contents.size();
		}
		std::size_t lineSize               (std::size_t index) const
		{
			return (index < contents.size() ? contents.at(index).size() : 0);
		}
		void        loadFromFile           ()
		{
			std::fstream file(fileName, std::ios::in);
			clearContents();
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(contents.size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFile           (const std::string & str)
		{
			std::fstream file(str, std::ios::in);
			clearContents();
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(contents.size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFileAndAppend  ()
		{
			std::fstream file(fileName, std::ios::in);
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(contents.size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFileAndAppend  (const std::string & filePath)
		{
			std::fstream file(filePath, std::ios::in);
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(contents.size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        outputToStream         (std::ostream & ostr) const
		{
			for (auto & i : contents)
			{
				for (double j : i)
				{
					ostr << j << " ";
				}
				ostr << std::endl;
			}
		}
		void        outputToFile           () const
		{
			std::fstream file(fileName, std::ios::out);
			for (const auto & i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        outputToFile           (const std::string & filePath) const
		{
			std::fstream file(filePath, std::ios::out);
			for (const auto & i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        appendToFile           () const
		{
			std::fstream file(fileName, std::ios::out | std::ios::app);
			for (auto & i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        appendToFile           (const std::string & filePath) const
		{
			std::fstream file(filePath, std::ios::out | std::ios::app);
			for (auto & i : contents)
			{
				for (double j : i)
				{
					if (file.is_open())
					{
						file << j << " ";
					}
				}
				if (file.is_open())
				{
					file << std::endl;
				}
			}
		}
		void        applyFunctionToEntry   (std::size_t line, std::size_t index, const std::function<double (double)> & function)
		{
			if (line < contents.size() && index < contents.at(line).size())
			{
				contents.at(line).at(index) = function(contents.at(line).at(index));
			}
		}
		void        applyFunctionToLine    (std::size_t line, const std::function<double (double)> & function)
		{
			if (line < contents.size())
			{
				for (double & i : contents.at(line))
				{
					i = function(i);
				}
			}
		}
		void        applyFunctionToContents(const std::function<double (double)> & function)
		{
			for (auto & i : contents)
			{
				for (double & j : i)
				{
					j = function(j);
				}
			}
		}
		void        sortLine               (std::size_t line, const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			if (line < contents.size())
			{
				std::sort(contents.at(line).begin(), contents.at(line).end(), predicate);
			}
		}
		void        sortLines              (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			if (upperBound < lowerBound)
			{
				std::swap(upperBound, lowerBound);
			}
			for (unsigned int i = lowerBound; i < contents.size() && i <= upperBound; ++i)
			{
				std::sort(contents.at(i).begin(), contents.at(i).end(), predicate);
			}
		}
		void        sortContents           (const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			for (auto & i : contents)
			{
				std::sort(i.begin(), i.end(), predicate);
			}
		}
		// Computational Utilities
		double computeValueFromLine                     (std::size_t line, const std::function<double (const std::deque<double> &)> & function)
		{
			return line < contents.size() ? function(contents.at(line)) : 0;
		}
		double computeValueFromLineUsingIteratorFunction(std::size_t line, const std::function<double (NumericLineIterator, NumericLineIterator)> & function)
		{
			return line < contents.size() ? function(contents.at(line).begin(), contents.at(line).end()) : 0;
		}
		// Iterators
		NumericFileIterator             begin  ()
		{
			return contents.begin();
		}
		NumericFileIterator             end    ()
		{
			return contents.end();
		}
		ConstNumericFileIterator        cbegin () const
		{
			return contents.cbegin();
		}
		ConstNumericFileIterator        cend   () const
		{
			return contents.cend();
		}
		ReverseNumericFileIterator      rbegin ()
		{
			return contents.rbegin();
		}
		ReverseNumericFileIterator      rend   ()
		{
			return contents.rend();
		}
		ConstReverseNumericFileIterator crbegin() const
		{
			return contents.crbegin();
		}
		ConstReverseNumericFileIterator crend  () const
		{
			return contents.crend();
		}
		// Overloaded Operators
		NumericFile &       operator =  (const NumericFile & rhs)
		{
			// Assignment operator
			contents = rhs.getFileContents();
			fileName = rhs.getFileName();
			closingAction = rhs.getClosingAction();
			return *this;
		}
		NumericFile &       operator =  (NumericFile && rhs)
		{
			// Move assignment operator
			contents = std::move(rhs.getFileContents());
			fileName = std::move(rhs.getFileName());
			closingAction = std::move(rhs.getClosingAction());
			return *this;
		}
		bool                operator == (const NumericFile & rhs) const
		{
			// Equivalence operator
			return contents == rhs.getFileContents() && fileName == rhs.getFileName() && closingAction == rhs.getClosingAction();
		}
		bool                operator != (const NumericFile & rhs) const
		{
			// Inequivalence operator
			return contents != rhs.getFileContents() || fileName != rhs.getFileName() || closingAction != rhs.getClosingAction();
		}
		const NumericLine & operator [] (std::size_t line) const
		{
			// Subscript operator
			return contents.at(line);
		}
		NumericLine         operator [] (std::size_t line)
		{
			// Subscript operator
			return contents.at(line);
		}
	};
}