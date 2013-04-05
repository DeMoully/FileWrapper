#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <algorithm>
#include <functional>
#include <string>
#include <numeric>

#include "FileCloseAction.hpp"
#include "CommonFunctions.hpp"

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
			// Perform an action based on the value of closingAction
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
			// Returns the entry at (line, index) if the entry exists, otherwise returns 0
			return (line < size() && index < lineSize(line)) ? contents.at(line).at(index) : 0;
		}
		NumericLine                            getLine                 (std::size_t line) const
		{
			// Returns the line at (line) if it exists, otherwise returns an empty NumericLine
			return (line < size()) ? contents.at(line) : NumericLine();
		}
		const std::deque<NumericLine> &        getFileContents         () const
		{
			// Returns the content of the file as a deque of NumericLines
			return contents;
		}
		std::string                            getFileName             () const
		{
			// Returns the fileName associated with the NumericFile
			return fileName;
		}
		FileCloseAction                        getClosingAction        () const
		{
			// Returns the closing action
			return closingAction;
		}
		std::string                            getClosingActionAsString() const
		{
			// Returns the closing action as a string
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
			// Sets fileName to filePath
			fileName = filePath;
		}
		void setClosingAction  (FileCloseAction onClose)
		{
			// Changes the closing action to onClose
			closingAction = onClose;
		}
		void setEntry          (std::size_t line, std::size_t index, double value)
		{
			// Sets the entry located at (line, index) to value
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).at(index) = value;
			}
		}
		void appendEntryToLine (std::size_t line, double value)
		{
			// Appends an entry to the line at (line) if it exists
			if (line < size())
			{
				contents.at(line).push_back(value);
			}
		}
		void prependEntryToLine(std::size_t line, double value)
		{
			// Prepends an entry to the line at (line) if it exists
			if (line < size())
			{
				contents.at(line).push_front(value);
			}
		}
		void insertEntryInLine (std::size_t line, std::size_t index, double value)
		{
			// Inserts an entry at (line, index) if possible
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).insert(contents.at(line).begin() + index, value);
			}
		}
		void appendLineToFile  (NumericLineIterator first, NumericLineIterator last)
		{
			// Appends a line to the file
			contents.emplace_back(first, last);
		}
		void prependLineToFile (NumericLineIterator first, NumericLineIterator last)
		{
			// Prepends a line to the file
			contents.emplace_front(first, last);
		}
		void insertLineInFile  (std::size_t line, NumericLineIterator first, NumericLineIterator last)
		{
			// Inserts a line into the file if possible
			if (line < size())
			{
				contents.emplace(contents.begin() + line, first, last);
			}
		}
		void appendLineToFile  (const NumericLine & line)
		{
			// Appends a line to the file
			contents.push_back(line);
		}
		void prependLineToFile (const NumericLine & line)
		{
			// Prepends a line to the file
			contents.push_front(line);
		}
		void insertLineInFile  (std::size_t line, const NumericLine & numericLine)
		{
			// Inserts a line into the file if possible
			if (line < size())
			{
				contents.insert(contents.begin() + line, numericLine);
			}
		}
		void removeEntry       (std::size_t line, std::size_t index)
		{
			// Removes an entry from the file if it exists
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).erase(contents.at(line).begin() + index);
			}
		}
		void removeLine        (std::size_t line)
		{
			// Removes a line from the file
			if (line < size())
			{
				contents.erase(contents.begin() + line);
			}
		}
		void clearContents     ()
		{
			// Clears the contents of the file
			contents.erase(contents.begin(), contents.end());
		}
		// Utilities
		bool        empty                        () const
		{
			return contents.empty();
		}
		std::size_t size                         () const
		{
			return contents.size();
		}
		std::size_t lineSize                     (std::size_t index) const
		{
			return (index < size() ? contents.at(index).size() : 0);
		}
		void        loadFromFile                 ()
		{
			std::fstream file(fileName, std::ios::in);
			clearContents();
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFile                 (const std::string & str)
		{
			std::fstream file(str, std::ios::in);
			clearContents();
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFileAndAppend        ()
		{
			std::fstream file(fileName, std::ios::in);
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        loadFromFileAndAppend        (const std::string & filePath)
		{
			std::fstream file(filePath, std::ios::in);
			contents.push_back(NumericLine());
			if (file.is_open())
			{
				double buffer;
				while (file >> buffer)
				{
					contents.at(size() - 1).push_back(buffer);
					if (file.peek() == '\n')
					{
						contents.push_back(NumericLine());
					}
				}
			}
		}
		void        outputToStream               (std::ostream & ostr) const
		{
			for (NumericLine i : contents)
			{
				for (double j : i)
				{
					if (ostr.good())
					{
						ostr << j << " ";
					}
				}
				ostr << std::endl;
			}
		}
		void        outputToFile                 () const
		{
			std::fstream file(fileName, std::ios::out);
			for (NumericLine i : contents)
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
		void        outputToFile                 (const std::string & filePath) const
		{
			std::fstream file(filePath, std::ios::out);
			for (NumericLine i : contents)
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
		void        appendToFile                 () const
		{
			std::fstream file(fileName, std::ios::out | std::ios::app);
			for (NumericLine i : contents)
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
		void        appendToFile                 (const std::string & filePath) const
		{
			std::fstream file(filePath, std::ios::out | std::ios::app);
			for (NumericLine i : contents)
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
		void        applyFunctionToEntry         (std::size_t line, std::size_t index, const std::function<double (double)> & function)
		{
			if (line < size() && index < lineSize(line))
			{
				contents.at(line).at(index) = function(contents.at(line).at(index));
			}
		}
		void        applyFunctionToEntries       (std::size_t line, std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			if (line < size())
			{
				FWPF::validateBounds(lowerBound, upperBound);
				for (unsigned int i = lowerBound; i <= upperBound && i < lineSize(line); ++i)
				{
					contents.at(line).at(i) = function(contents.at(line).at(i));
				}
			}
		}
		void        applyFunctionToEntryInLines  (std::size_t entry, std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				if (entry < lineSize(i))
				{
					contents.at(i).at(entry) = function(contents.at(i).at(entry));
				}
			}
		}
		void        applyFunctionToEntriesInLines(std::size_t lowerEntry, std::size_t upperEntry, std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			FWPF::validateBounds(lowerEntry, upperEntry);
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				for (unsigned int j = lowerEntry; j < upperEntry && j < lineSize(i); ++j)
				{
					contents.at(i).at(j) = function(contents.at(i).at(j));
				}
			}
		}
		void        applyFunctionToLine          (std::size_t line, const std::function<double (double)> & function)
		{
			if (line < size())
			{
				for (double & i : contents.at(line))
				{
					i = function(i);
				}
			}
		}
		void        applyFunctionToLines         (std::size_t lowerBound, std::size_t upperBound, const std::function<double (double)> & function)
		{
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				for (double & j : contents.at(i))
				{
					j = function(j);
				}
			}
		}
		void        applyFunctionToContents      (const std::function<double (double)> & function)
		{
			for (NumericLine & i : contents)
			{
				for (double & j : i)
				{
					j = function(j);
				}
			}
		}
		void        sortLine                     (std::size_t line, const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			if (line < size())
			{
				std::sort(contents.at(line).begin(), contents.at(line).end(), predicate);
			}
		}
		void        sortLines                    (std::size_t lowerBound, std::size_t upperBound, const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			FWPF::validateBounds(lowerBound, upperBound);
			for (unsigned int i = lowerBound; i < size() && i <= upperBound; ++i)
			{
				std::sort(contents.at(i).begin(), contents.at(i).end(), predicate);
			}
		}
		void        sortContents                 (const std::function<bool (double, double)> & predicate = std::less<double>())
		{
			for (NumericLine & i : contents)
			{
				std::sort(i.begin(), i.end(), predicate);
			}
		}
		// Computational Utilities
		double computeValueFromLine                     (std::size_t line, const std::function<double (const std::deque<double> &)> & function)
		{
			return line < size() ? function(contents.at(line)) : 0;
		}
		double computeValueFromLineUsingIteratorFunction(std::size_t line, const std::function<double (NumericLineIterator, NumericLineIterator)> & function)
		{
			return line < size() ? function(contents.at(line).begin(), contents.at(line).end()) : 0;
		}
		double computeSumOfLine                         (std::size_t line) const
		{
			return line < size() ? std::accumulate(contents.at(line).begin(), contents.at(line).end(), 0.0) : 0;
		}
		double computeSumOfLines                        (std::size_t lowerBound, std::size_t upperBound) const
		{
			FWPF::validateBounds(lowerBound, upperBound);
			double sum = 0;
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				sum += std::accumulate(contents.at(i).begin(), contents.at(i).end(), 0.0);
			}
			return sum;
		}
		double computeSumOfContents                     () const
		{
			double sum = 0;
			for (NumericLine i : contents)
			{
				sum += std::accumulate(i.begin(), i.end(), 0.0);
			}
			return sum;
		}
		double computeAverageOfLine                     (std::size_t line) const
		{
			return lineSize(line) ? std::accumulate(contents.at(line).begin(), contents.at(line).end(), 0.0) / lineSize(line) : 0;
		}
		double computeAverageOfLines                    (std::size_t lowerBound, std::size_t upperBound) const
		{
			FWPF::validateBounds(lowerBound, upperBound);
			double sum = 0;
			std::size_t numElems = 0;
			for (unsigned int i = lowerBound; i <= upperBound; ++i)
			{
				sum += std::accumulate(contents.at(i).begin(), contents.at(i).end(), 0.0);
				numElems += lineSize(i);
			}
			return numElems ? sum / numElems : 0;
		}
		double computeAverageOfContents                 () const
		{
			double sum = 0;
			std::size_t numElems = 0;
			for (unsigned int i = 0; i < size(); ++i)
			{
				sum += std::accumulate(contents.at(i).begin(), contents.at(i).end(), 0.0);
				numElems += lineSize(i);
			}
			return numElems ? sum / numElems : 0;
		}
		double computeVarianceOfLine                    (std::size_t line) const
		{
			if (line < size())
			{
				if (lineSize(line))
				{
					double mean = computeAverageOfLine(line);
					double sumOfSquares = 0;
					for (double i : contents.at(line))
					{
						sumOfSquares += (i - mean) * (i - mean);
					}
					return sumOfSquares / lineSize(line);
				}
				return 0;
			}
			return 0;
		}
		double computeVarianceOfLines                   (std::size_t lowerBound, std::size_t upperBound) const
		{
			FWPF::validateBounds(lowerBound, upperBound);
			double mean = computeAverageOfLines(lowerBound, upperBound);
			double sumOfSquares = 0;
			std::size_t numElems = 0;
			for (unsigned int i = lowerBound; i <= upperBound && i < size(); ++i)
			{
				for (unsigned int j = 0; j < lineSize(i); ++j)
				{
					sumOfSquares += (contents.at(i).at(j) - mean) * (contents.at(i).at(j) - mean);
					++numElems;
				}
			}
			return numElems ? sumOfSquares / numElems : 0;
		}
		double computeStandardDeviationOfLine           (std::size_t line) const
		{
			return std::sqrt(computeVarianceOfLine(line));
		}
		double computeStandardDeviationOfLines          (std::size_t lowerBound, std::size_t upperBound) const
		{
			return std::sqrt(computeVarianceOfLines(lowerBound, upperBound));
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