#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>
#include <algorithm>
#include <typeinfo>
#include <iostream>

using namespace std;

class BaseFile {
private:
	string name;

public:
	virtual ~BaseFile() ;
	BaseFile(string name);
	string getName() const;
	void setName(string newName);
	virtual int getSize() = 0;
	virtual bool isDir()=0;
};

class File : public BaseFile {
private:
	int size;

public:
	File(string name, int size); // Constructor
	int getSize(); // Return the size of the file
	bool isDir();

};

class Directory : public BaseFile {
private:

	Directory *parent;
	vector<BaseFile*> children;
	bool isUniqueName(BaseFile* file); //Check if file exists in children list
	void clear(); //to use for dtor
	void copy(const Directory &rhs); //to use for copy ctor


public:
	Directory(string name, Directory *parent); // Constructor
	Directory(const Directory &rhs); // Copy Constructor
	virtual ~Directory();//----------------------Destructor
	Directory & operator=(const Directory &rhs); //Operator =
	Directory(Directory &&rhs); // Move Constructor
	Directory & operator=(Directory &&rhs); // Move Assignment Operator
	Directory *getParent() const; // Return a pointer to the parent of this directory
	void setParent(Directory *newParent); // Change the parent of this directory
	void addFile(BaseFile* file); // Add the file to children
	void removeFile(string name); // Remove the file with the specified name from children
	void removeFile(BaseFile* file); // Remove the file from children
	void sortByName(); // Sort children by name alphabetically (not recursively)
	void sortBySize(); // Sort children by size (not recursively)
	vector<BaseFile*> getChildren(); // Return children
	int getSize(); // Return the size of the directory (recursively)
	string getAbsolutePath();  //Return the path from the root to this
	bool isDir();


};

#endif
