*What is the FileWrapper class?

The FileWrapper class is an object-oriented solution to File I/O with C++. fstreams are tedious and error-prone. By wrapping fstream's functionality into a simple and easy to use wrapper, FileWrapper allows its users to get past the hurdles associated with setting up File I/O for a program, leaving them to solve whatever problem they are tackling.

*Who should use the FileWrapper class?

Anyone who needs access to efficient and simple File I/O. The FileWrapper class is versatile, robust, and intuitive, making it a simple and elegant solution to a wide variety of problems.

*What do I need in order to use the FileWrapper class?

Just the header file that declares and defines the class and a C++11 compliant compiler. That's it. There are no other requirements. Simple, right?

*What can't the FileWrapper class do?

The FileWrapper class was designed to store the contents of files in a string format. As such, it is not suited to perform binary I/O. Additionally, it cannot rename, delete, or modify files in any way other than changing their contents. Boost's Filesystem or some of the functions provided by the Standard Library may be what you're looking for if this is what you need.

*What can the FileWrapper class do?

The main use for the FileWrapper class is File I/O. Reading in the contents of a file is as simple as calling a single function. In addition to that, the FileWrapper class provides easy and elegant functions for modifying the contents of files, applying functions to ranges of lines in a file, iterating through a file line by line using Standard Compliant iterators, and much more. If you need to do something to a plain-text file, chances are the FileWrapper class is capable of doing it.

