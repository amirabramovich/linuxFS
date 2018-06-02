/*
 * FileSystem.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: amir
 */

#include "../include/FileSystem.h"
#include "../include/GlobalVariables.h"
#include <iostream>

using namespace std;



//constructor
FileSystem::FileSystem(void):rootDirectory(),workingDirectory(){
	Directory* root=new Directory("/",nullptr);
	this->rootDirectory=root;
	this->workingDirectory=root;
}

void FileSystem::copy(const FileSystem &rhs){
	Directory* newroot= new Directory(rhs.getRootDirectory());
	this->rootDirectory=newroot;
	this->workingDirectory=rootDirectory;
	string path=rhs.getWorkingDirectory().getAbsolutePath();
	path=path.substr(1);
	while(path!=""){
		for(auto it:workingDirectory->getChildren()){
			if(it->getName()==path.substr(0,path.find_first_of('/'))){
				this->workingDirectory=(Directory*)it;
				path=path.substr(path.find_first_of('/')+1);
			}
		}
	}
}

FileSystem::FileSystem(const FileSystem &rhs):rootDirectory(nullptr),workingDirectory(nullptr){// Copy Constructor
	if((verbose==1)||(verbose==3))cout << "FileSystem FileSystem& FileSystem::copy(const FileSystem &rhs)" <<endl;
	copy(rhs);
}

FileSystem::FileSystem(FileSystem &&rhs):rootDirectory(nullptr),workingDirectory(nullptr){// Move Constructor
	if((verbose==1)||(verbose==3))cout << "FileSystem FileSystem::FileSystem(FileSystem &&rhs)" << endl;
	rhs.rootDirectory=nullptr;
	rhs.workingDirectory=nullptr;
}
FileSystem& FileSystem::operator=(FileSystem &&rhs){// Move Assignment Operator
	if((verbose==1)||(verbose==3))cout << "FileSystem FileSystem::FileSystem& operator=(FileSystem &&rhs)" << endl;
	if (this != &rhs)
	{
		clear();
		this->rootDirectory = rhs.rootDirectory;
		this->workingDirectory=rhs.workingDirectory;
		rhs.rootDirectory=nullptr;
		rhs.workingDirectory=nullptr;
	}
	return *this;
}

void FileSystem::clear(){
	delete rootDirectory;
	rootDirectory=nullptr;
	workingDirectory=nullptr;
}

FileSystem:: ~FileSystem(){//Destructor
	if((verbose==1)||(verbose==3))cout << "FileSystem FileSystem:: ~FileSystem()" <<endl;
	clear();}

FileSystem & FileSystem::operator=(const FileSystem &rhs){//Operator =
	if((verbose==1)||(verbose==3))cout << "FileSystem FileSystem & FileSystem::operator=(const FileSystem &rhs)" <<endl;
	if(this==&rhs)return *this;
	clear();
	copy(rhs);
	return *this;
}

Directory& FileSystem::getRootDirectory() const{// Return reference to the root directory
	Directory& root=*rootDirectory;
	return root;
}
Directory& FileSystem::getWorkingDirectory() const{// Return reference to the working directory
	Directory& working=*workingDirectory;
	return working;
}
void FileSystem::setWorkingDirectory(Directory *newWorkingDirectory){// Change the working directory of the file system
	workingDirectory=newWorkingDirectory;
}

