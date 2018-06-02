/*
 * Files.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: amir
 */

#include "../include/Files.h"
#include "../include/GlobalVariables.h"



using namespace std;


BaseFile::BaseFile(string name): name(name){}
BaseFile::~BaseFile() {}

string BaseFile::getName() const{
	return name;
}
void BaseFile::setName(string newName){
	name=newName;
}

File::File(string name, int size): BaseFile(name),size(size){}

int File::getSize(){
	return size;
}
bool File::isDir(){return false;}

Directory::Directory(string name, Directory* parent): BaseFile(name),parent(parent),children(){}

bool Directory::isDir(){return true;}

void Directory::clear(){
	for(auto it:children){
			delete (it);
	}
    children.clear();
}

Directory::~Directory() {
	if((verbose==1)||(verbose==3))cout << "Directory Directory::~Directory()" << endl;
	clear();
}

void Directory::copy(const Directory &rhs){ //deep copy of rhs Directory
    this->setName(rhs.getName());
    this->setParent(rhs.getParent());
    for(auto it:rhs.children){
    	if(it->isDir()==false){
    		this->children.push_back(new File(*((File*)it)));}
    	else if(it->isDir()){
    		Directory *tmp=new Directory(*((Directory*)it));
    		this->children.push_back(tmp);
			tmp->setParent(this);
    	}
    }
}


Directory::Directory(const Directory &rhs):BaseFile("tmp"),parent(nullptr),children(){// Copy Constructor
	if((verbose==1)||(verbose==3))cout << "Directory Directory::Directory(const Directory &rhs)" << endl;
	copy(rhs);
}

Directory& Directory::operator=(const Directory &rhs){//Operator =
	if((verbose==1)||(verbose==3))cout << "Directory Directory& Directory::operator=(const Directory &rhs)" << endl;
	if(this!=&rhs){
	clear();
	copy(rhs);
	}
	return *this;
}

Directory::Directory(Directory &&rhs):BaseFile("tmp"),parent(nullptr),children(){// Move Constructor
	if((verbose==1)||(verbose==3))cout << "Directory Directory::Directory(Directory &&rhs)" << endl;
	rhs.children.clear();
	rhs.parent=nullptr;

}
Directory& Directory::operator=(Directory &&rhs){// Move Assignment Operator
	if((verbose==1)||(verbose==3))cout << "Directory Directory& Directory::operator=(Directory &&rhs)" << endl;
	if (this != &rhs)
	    {
	        clear();
	        this->children = rhs.children;
	        this->parent=rhs.parent;
	        rhs.children.clear();
	        rhs.parent = nullptr;
	    }
	return *this;
}

Directory* Directory::getParent() const{
	return parent;
}

void Directory::setParent(Directory *newParent){
	parent=newParent;
}
bool Directory::isUniqueName(BaseFile* file){ //Check if file exists in children list
	for(auto it:children){
		if(it->getName()==file->getName()){
			return false;}
	}
	return true;
}
void Directory::addFile(BaseFile* file){// Add the file to children
	if (isUniqueName(file)==false){
		if(file->isDir()) cout << "The directory already exists" << endl;
		else cout << "File already exists" << endl;
		return;
	}
	children.push_back(file);
	if(file->isDir()){
		((Directory*)file)->setParent(this);
	}
}
void Directory::removeFile(string name){// Remove the file with the specified name from children
	bool flag=false;
	int i=0;
	for(auto it:children){
		if(it->getName()==name){
			if(it->isDir()){
					((Directory*)it)->clear();
			}
			delete it;
			children.erase(children.begin()+i); //delete the pointer
			flag=true;
		}
		i++;
	}
	if(!flag) cout << "No such file or directory" << endl;
}
void Directory::removeFile(BaseFile* file){// Remove the file from children
	bool flag=false;
	int i=0;
	for(auto it:children){
		if(it==file){
			if(it->isDir()){
				((Directory*)it)->clear();
			}
			delete it;
			children.erase(children.begin()+i); //delete the pointer
			flag=true;
		}
		i++;
	}
	if(!flag) cout << "No such file or directory" << endl;
}


	void Directory::sortByName(){// Sort children by name alphabetically (not recursively)
		sort(children.begin(), children.end(),[](BaseFile* lhs, BaseFile* rhs){
		    return lhs->getName() < rhs->getName();
		});
	}



	void Directory::sortBySize(){ // Sort children by size (not recursively)
		sort(children.begin(), children.end(),[](BaseFile* lhs, BaseFile* rhs){
			if(lhs->getSize() == rhs->getSize()){return (lhs->getName() < rhs->getName());}
			return lhs->getSize() < rhs->getSize();
		});
	}

	vector<BaseFile*> Directory::getChildren(){ return children;} // Return children

	int Directory::getSize(){// Return the size of the directory (recursively)
		int sum=0;
		for(auto it:children){
			sum=sum+it->getSize();
		}
		return sum;
	}

	string Directory::getAbsolutePath(){//Return the path from the root to this
		string str=this->getName();
		Directory* tmp=this;
		if(tmp->getName()=="/") return str;
		while(tmp->getName()!="/"){
			str.insert(0,tmp->getParent()->getName()+"/");
			tmp=tmp->getParent();
		}
		return str.substr(1);//to remove first '/'
	}

