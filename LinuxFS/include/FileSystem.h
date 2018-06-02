#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include "Files.h"


class FileSystem {
private:
	Directory* rootDirectory;
	Directory* workingDirectory;
	void clear();
	void copy(const FileSystem &rhs);
public:
	FileSystem();
	FileSystem(const FileSystem &rhs); // Copy Constructor
	virtual ~FileSystem();//----------------------Destructor
	FileSystem & operator=(const FileSystem &rhs); //Operator =
	FileSystem(FileSystem &&rhs); // Move Constructor
	FileSystem & operator=(FileSystem &&rhs); // Move Assignment Operator
	Directory& getRootDirectory() const; // Return reference to the root directory
	Directory& getWorkingDirectory() const; // Return reference to the working directory
	void setWorkingDirectory(Directory *newWorkingDirectory); // Change the working directory of the file system


};


#endif
