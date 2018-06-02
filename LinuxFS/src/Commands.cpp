/*
 * Commands.cpp
 *
 *  Created on: Nov 10, 2017
 *      Author: amir
 */

#include "../include/Commands.h"
#include "../include/GlobalVariables.h"
#include <iostream>

using namespace std;



BaseCommand::BaseCommand(string args): args(args){}

string BaseCommand::getArgs(){
	return args;
}

BaseCommand::~BaseCommand() {}

PwdCommand::PwdCommand(string args): BaseCommand(args){}

PwdCommand::~PwdCommand() {}

void PwdCommand::execute(FileSystem & fs){

	cout << fs.getWorkingDirectory().getAbsolutePath() << endl;
}

string PwdCommand::toString(){return "pwd";}


CdCommand::CdCommand(string args):BaseCommand(args){}
CdCommand::~CdCommand() {}
void CdCommand::execute(FileSystem & fs){

	execute(fs,getArgs());
}
//Receive path, switch work dir to the last dir available in the path and returns it
Directory* BaseCommand::tryToGoTo(FileSystem & fs,string s){
	if(s==""){
		return &(fs.getWorkingDirectory());
	}
	else if(s=="/"){fs.setWorkingDirectory(&fs.getRootDirectory());
		return &(fs.getRootDirectory());
	}
	else if(s==".."){
		if(&fs.getWorkingDirectory()!=&fs.getRootDirectory()){
			fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
			return (&fs.getWorkingDirectory());
		}
		else
			return &(fs.getRootDirectory());
	}
	else if(s.at(0)=='/'){
		fs.setWorkingDirectory(&(fs.getRootDirectory()));
		return tryToGoTo(fs,s.substr(1));
	}
	else if((s.size()>2)&&(s.substr(0,2)=="..")){
		if(&fs.getWorkingDirectory()!=&fs.getRootDirectory()){
			fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
			return tryToGoTo(fs,s.substr(3));
		}
	}
	else{
		for(auto it:(fs.getWorkingDirectory().getChildren())){
			if((it->isDir())&&((((it->getName()==s.substr(0,s.find_first_of('/'))||(it->getName()==s)))))){//found child with the name
				fs.setWorkingDirectory((Directory*)it);
				if(s.find_first_of('/')>0&&s.find_first_of('/')<s.size()) return tryToGoTo(fs,s.substr(s.find_first_of('/')+1));
				else  return &(fs.getWorkingDirectory());
			}
		}
		return &(fs.getWorkingDirectory());//did not find child with the name
	}
	return &(fs.getWorkingDirectory());
}
//Receive path, returns the last dir available in the path without switching dir
Directory* BaseCommand::stableTryToGoTo(FileSystem & fs,string s){
	Directory* d=&(fs.getWorkingDirectory());
	Directory* dir=tryToGoTo(fs,s);
	if(&fs.getWorkingDirectory()!=d)
		fs.setWorkingDirectory(d);
	return dir;
}



//Receive path, switch work dir to the last dir available in the path and returns true if path leads to dir
bool BaseCommand::isPathLegal(FileSystem & fs,string s,bool dir){
	if(s==""){
		return true;
	}
	else if(s=="/"){fs.setWorkingDirectory(&fs.getRootDirectory());
		return true;
	}
	else if(s==".."){
		if(&fs.getWorkingDirectory()!=&fs.getRootDirectory())
			return true;
		else
			return false;
	}
	else if(s.at(0)=='/'){
		fs.setWorkingDirectory(&(fs.getRootDirectory()));
		return isPathLegal(fs,s.substr(1),dir);
	}
	else if((s.size()>2)&&(s.substr(0,2)=="..")){
		if(&fs.getWorkingDirectory()!=&fs.getRootDirectory()){
			fs.setWorkingDirectory(fs.getWorkingDirectory().getParent());
			return isPathLegal(fs,s.substr(3),dir);
		}
	}
	else{
		for(auto it:(fs.getWorkingDirectory().getChildren())){
			if((it->isDir())&&((((it->getName()==s.substr(0,s.find_first_of('/'))||(it->getName()==s)))))){//found child dir with the name
				fs.setWorkingDirectory((Directory*)it);
				if(s.find_first_of('/')>0&&s.find_first_of('/')<s.size()) return isPathLegal(fs,s.substr(s.find_first_of('/')+1),dir);
				else return true;
			}
			else if((it->isDir()==false)&&((((it->getName()==s.substr(0,s.find_first_of('/'))||(it->getName()==s)))))){//found child file with the name
				return dir==false;
			}
		}
		return false;//did not find child with the name
	}
	return false;
}


bool BaseCommand::stableIsPathLegal(FileSystem & fs,string s,bool dir){
	Directory* d=&(fs.getWorkingDirectory());
	bool b=isPathLegal(fs,s,dir);
	fs.setWorkingDirectory(d);
	return b;
}

void CdCommand::execute(FileSystem & fs,string s){
	if(stableIsPathLegal(fs,s,1))
		fs.setWorkingDirectory(tryToGoTo(fs,s));
	else
		cout << "The system cannot find the path specified" << endl;
}

string CdCommand::toString(){return "cd";}

LsCommand::LsCommand(string args):BaseCommand(args){}
LsCommand::~LsCommand() {}

void LsCommand::execute(FileSystem & fs){

	if(getArgs().substr(0,2)=="-s"){ //in case -s flag
		string path=getArgs();
		if(getArgs()=="-s")
			path=getArgs().substr(2);
		else
			path=getArgs().substr(3);
		if(stableIsPathLegal(fs,path,1)==false){
			cout << "The system cannot find the path specified" << endl;
			return;
		}
		Directory* tmp=stableTryToGoTo(fs,path);
		tmp->sortBySize();
		for(auto i:tmp->getChildren()){
			string s;
			if(i->isDir()){s="DIR";}
			else{s="FILE";}
			cout << s<<'\t'<<i->getName()<<'\t'<<i->getSize() << endl;
		}
	}
	else{ //dont have -s flag and have path
		if(stableIsPathLegal(fs,getArgs(),1)==false){
			cout << "The system cannot find the path specified" << endl;
			return;
		}
		Directory* tmp=stableTryToGoTo(fs,getArgs());
		tmp->sortByName();

		for(auto i:tmp->getChildren()){
			string s;
			if(i->isDir()){s="DIR";}
			else{s="FILE";}
			cout << s<<'\t'<<i->getName()<<'\t'<<i->getSize() << endl;
		}
	}
}
string LsCommand::toString(){return "ls";}
MkdirCommand::~MkdirCommand() {}
MkdirCommand::MkdirCommand(string args):BaseCommand(args){}
void MkdirCommand::execute(FileSystem & fs){

	string pathToDir=getArgs();//path from root to new dir - assume input is from root
		if(pathToDir.at(0)!='/'){//if path in input is not from root but from working dir
			if(fs.getWorkingDirectory().getName()!="/")
				pathToDir=fs.getWorkingDirectory().getAbsolutePath()+'/'+pathToDir;
			else
				pathToDir=fs.getWorkingDirectory().getAbsolutePath()+pathToDir;
		}
	if((stableIsPathLegal(fs,pathToDir,0))||(stableIsPathLegal(fs,pathToDir,1))){
		cout<<"The directory already exists"<<endl;
		return;
	}
	Directory *d=stableTryToGoTo(fs,getArgs());
	string pathToLastDir=d->getAbsolutePath(); //path from root to last dir in chain
	int buildPathLength=pathToDir.size()-pathToLastDir.size();
	int buildPathBegin=pathToLastDir.size();
	string buildPath=pathToDir.substr(buildPathBegin,buildPathLength);//the folders which need to be built
	if(buildPath.at(0)=='/')buildPath=buildPath.substr(1);
	while(buildPath!=""){
		string folder;
		size_t from=0;
		size_t to=buildPath.find_first_of('/');
		if((to>0)&&(to<buildPath.size())){
			folder=buildPath.substr(from,to);
			buildPath=buildPath.substr(to+1);
		}
		else{
			folder=buildPath.substr(0);
			buildPath="";
		}
		if(folder.at(0)=='/')
			folder=folder.substr(1);
		Directory* newDir=new Directory(folder,d);
		d->addFile(newDir);
		d=newDir;
	}
}
string MkdirCommand::toString(){return "mkdir";}
MkfileCommand::~MkfileCommand() {}
MkfileCommand::MkfileCommand(string args):BaseCommand(args){}
void MkfileCommand::execute(FileSystem & fs){

	int size = 0;
	string str=(getArgs().substr(getArgs().find_last_of(' ')+1)); // stoi does not work :( try to convert string to int
	for_each(str.begin(), str.end(), [&size](char c){ size = 10 * size + (c - '0');  });
	string pathInput=getArgs().substr(0,getArgs().find_last_of(' '));//without size
	string fileName=pathInput.substr(pathInput.find_last_of('/')+1);//get name
	if(pathInput.find_last_of('/')>0&&pathInput.find_last_of('/')<pathInput.size())
	pathInput=pathInput.substr(0,pathInput.find_last_of('/'));//path without file name
	else if(pathInput.find_last_of('/')==0)
		pathInput="/";
	else
		pathInput=fs.getWorkingDirectory().getAbsolutePath();
	Directory *d=stableTryToGoTo(fs,pathInput);
	string pathToLastDir=d->getAbsolutePath(); //path from root to last dir in chain
	if(pathToLastDir==pathInput||(pathToLastDir==fs.getWorkingDirectory().getName()+pathInput)){//folder has been found
		for(auto it:d->getChildren()){
			if(it->getName()==fileName){
				cout << "File already exists" << endl;return;
			}
		}
		File* f=new File(fileName, size);
		d->addFile(f);
		return;
	}
	cout << "The system cannot find the path specified"<< endl;
}
string MkfileCommand::toString(){return "mkfile";}
CpCommand::~CpCommand() {}

CpCommand::CpCommand(string args):BaseCommand(args){}

bool CpCommand::illegalParent(FileSystem & fs, Directory* D){//return true if D parent of working dir or root
	if(D==&fs.getRootDirectory())return true;
	while(D->getParent()!=nullptr){
		if(D->getParent()==&fs.getWorkingDirectory())
			return false;
		D=D->getParent();
	}
	return true;
}


void CpCommand::execute(FileSystem & fs){

	if((stableIsPathLegal(fs,getArgs().substr(0,getArgs().find_first_of(' ')),0)==false)
	&&(stableIsPathLegal(fs,getArgs().substr(0,getArgs().find_first_of(' ')),1)==false)){ //check if source path exist
		cout << "No such file or directory" << endl;
		return;
	}
	if(stableIsPathLegal(fs,getArgs().substr(0,getArgs().find_first_of(' ')),1)){//Found Folder
		Directory* directoryToCopy=stableTryToGoTo(fs,getArgs().substr(0,getArgs().find_first_of(' ')));
		if(illegalParent(fs, directoryToCopy)){
			cout << "Can't copy directory" << endl;
			return;
		}
		else{//copy the directory! first check is destination is legal
			if(stableIsPathLegal(fs,getArgs().substr(getArgs().find_first_of(' ')+1),1)==false){ //check if destination path exist
				cout << "No such file or directory" << endl;
				return;
			}//copy the directory!
			Directory* newparent=stableTryToGoTo(fs,getArgs().substr(getArgs().find_first_of(' ')+1));

			//check if folder/file existing in destination
			for(auto it:newparent->getChildren())
				if(it->getName()==directoryToCopy->getName()){
					return;//cout << "file already exists in destination" << endl;
				}
			Directory* D= new Directory(*directoryToCopy);
			newparent->addFile(D);
		}
	}
	else{//Found File
		Directory* parent=(stableTryToGoTo(fs,getArgs().substr(0,getArgs().find_first_of(' '))));
		string fullPath=getArgs().substr(0,getArgs().find_first_of(' '));
		if(fullPath.at(0)!='/')
			fullPath=parent->getAbsolutePath()+'/'+fullPath;
		string fileToDelete=fullPath.substr(fullPath.find_last_of('/')+1);
		if(stableIsPathLegal(fs,getArgs().substr(getArgs().find_first_of(' ')+1),1)==false){ //check if destination path exist
			cout << "No such file or directory" << endl;
			return;
		}//copy the file!
		Directory* newparent=stableTryToGoTo(fs,getArgs().substr(getArgs().find_first_of(' ')+1));
		string fname=fullPath.substr(fullPath.find_last_of('/')+1);
		fullPath=fullPath.substr(0,fullPath.find_last_of('/'));
		for(auto it:parent->getChildren()){
			if(it->getName()==fname){//found the file
				for(auto it:newparent->getChildren()){ //check if it is existing in destionation
					if(it->getName()==fname){
						return;//cout << "file already exists in destination" << endl;
					}
				}
				File* F=new File(fname,it->getSize());
				newparent->addFile(F);
			}
		}
	}
}
string CpCommand::toString(){return "cp";}



MvCommand::MvCommand(string args):BaseCommand(args){}
MvCommand::~MvCommand() {}

bool MvCommand::illegalParent(FileSystem & fs, Directory* D){//return true if D parent of working dir or root
	if(D==&fs.getRootDirectory())return true;
	while(D->getParent()!=nullptr){
		if(D->getParent()==&fs.getWorkingDirectory())
			return false;
		D=D->getParent();
	}
	return true;
}


void MvCommand::execute(FileSystem & fs){

	if((stableIsPathLegal(fs,getArgs().substr(0,getArgs().find_first_of(' ')),0)==false)
	&&(stableIsPathLegal(fs,getArgs().substr(0,getArgs().find_first_of(' ')),1)==false)){ //check if source path exist
		cout << "No such file or directory" << endl;
		return;
	}
	if(stableIsPathLegal(fs,getArgs().substr(0,getArgs().find_first_of(' ')),1)){//Found Folder
		Directory* directoryToMove=stableTryToGoTo(fs,getArgs().substr(0,getArgs().find_first_of(' ')));
		if(illegalParent(fs, directoryToMove)){
			cout << "Can't move directory" << endl;
			return;
		}
		else{//move the directory! first check is destination is legal
			if(stableIsPathLegal(fs,getArgs().substr(getArgs().find_first_of(' ')+1),1)==false){ //check if destination path exist
				cout << "No such file or directory" << endl;
				return;
			}//move the directory!
			Directory* newparent=stableTryToGoTo(fs,getArgs().substr(getArgs().find_first_of(' ')+1));
			//check if folder/file existing in destination
			for(auto it:newparent->getChildren())
				if(it->getName()==directoryToMove->getName()){
					return;//cout << "file already exists in destination" << endl;
				}
			Directory* D= new Directory(*directoryToMove);
			newparent->addFile(D);
			directoryToMove->getParent()->removeFile(directoryToMove);
		}
	}
	else{//Found File
		Directory* parent=(stableTryToGoTo(fs,getArgs().substr(0,getArgs().find_first_of(' '))));
		string fullPath=getArgs().substr(0,getArgs().find_first_of(' '));
		if(fullPath.at(0)!='/')
			fullPath=parent->getAbsolutePath()+'/'+fullPath;
		string fileToDelete=fullPath.substr(fullPath.find_last_of('/')+1);
		if(stableIsPathLegal(fs,getArgs().substr(getArgs().find_first_of(' ')+1),1)==false){ //check if destination path exist
			cout << "No such file or directory" << endl;
			return;
		}//move the file!
		Directory* newparent=stableTryToGoTo(fs,getArgs().substr(getArgs().find_first_of(' ')+1));
		string fname=fullPath.substr(fullPath.find_last_of('/')+1);
		fullPath=fullPath.substr(0,fullPath.find_last_of('/'));
		for(auto it:parent->getChildren()){
			if(it->getName()==fname){//found the file
				for(auto it:newparent->getChildren()){ //check if it is existing in destionation
					if(it->getName()==fname){
						return;//cout << "file already exists in destination" << endl;
					}
				}
				File* F=new File(fname,it->getSize());
				newparent->addFile(F);
				parent->removeFile(fname);
			}
		}
	}
}

string MvCommand::toString(){return "mv";}
RenameCommand::~RenameCommand() {}
RenameCommand::RenameCommand(string args):BaseCommand(args){}
bool RenameCommand::illegalParent(FileSystem & fs, Directory* D){//return true if D parent of working dir or root
	if(D==&fs.getRootDirectory())return true;
	while(D->getParent()!=nullptr){
		if(D->getParent()==&fs.getWorkingDirectory())
			return false;
		D=D->getParent();
	}
	return true;
}
void RenameCommand::execute(FileSystem & fs){

	string path=getArgs().substr(0,getArgs().find_first_of(' '));
	string newname=getArgs().substr(getArgs().find_first_of(' ')+1);
	if((stableIsPathLegal(fs,path,0)==false)&&(stableIsPathLegal(fs,path,1)==false)){
		cout << "No such file or directory" << endl;
		return;
	}
	if(stableIsPathLegal(fs,path,1)){//Found Folder
		Directory* directoryToRename=(stableTryToGoTo(fs,path));
		if(illegalParent(fs, directoryToRename)){
			cout << "Can't rename the working directory" << endl;
			return;
		}
		else{//rename the directory!
			for(auto it:directoryToRename->getParent()->getChildren())
						if(it->getName()==newname){
							//cout<<"The directory already exists"<<endl;
							return;///folder with same name exists
						}
			directoryToRename->setName(newname);}
	}
	else{//Found File
		Directory* parent=(stableTryToGoTo(fs,path));
		string fullPath=path;
		if(fullPath.at(0)!='/')
			fullPath=parent->getAbsolutePath()+'/'+path;
		string fileToRename=fullPath.substr(fullPath.find_last_of('/')+1);
		for(auto it:parent->getChildren())
			if(it->getName()==newname){
				//cout<<"The directory already exists"<<endl;
				return; ///file with same name exists
			}
		for(auto it:parent->getChildren())
			if(it->getName()==fileToRename)
				it->setName(newname);
		}
}
string RenameCommand::toString(){return "rename";}

RmCommand::~RmCommand() {}

RmCommand::RmCommand(string args):BaseCommand(args){}
bool RmCommand::illegalParent(FileSystem & fs, Directory* D){//return true if D parent of working dir or root
	if(D==&fs.getRootDirectory())return true;
	while(D->getParent()!=nullptr){
		if(D->getParent()==&fs.getWorkingDirectory())
			return false;
		D=D->getParent();
	}
	return true;
}
void RmCommand::execute(FileSystem & fs){

	if(stableIsPathLegal(fs,getArgs(),0)==false){
		cout << "No such file or directory" << endl;
		return;
	}
	if(stableIsPathLegal(fs,getArgs(),1)){//Found Folder
		Directory* directoryToDelete=(stableTryToGoTo(fs,getArgs()));
		if(illegalParent(fs, directoryToDelete)){
			cout << "Can't remove directory" << endl;
			return;
		}
		else//delete the directory!
			directoryToDelete->getParent()->removeFile(directoryToDelete);
	}
	else{//Found File
		Directory* parent=(stableTryToGoTo(fs,getArgs()));
		string fullPath=getArgs();
		if(fullPath.at(0)!='/')
			fullPath=parent->getAbsolutePath()+'/'+getArgs();
		string fileToDelete=fullPath.substr(fullPath.find_last_of('/')+1);
		parent->removeFile(fileToDelete);
		}
}
string RmCommand::toString(){return "rm";}


HistoryCommand::~HistoryCommand() {}
HistoryCommand::HistoryCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args),history(history){}
void HistoryCommand::execute(FileSystem & fs){

	int i=0;
	for(auto it:history){
		if((it->toString()=="pwd")||(it->toString()=="cd")||(it->toString()=="ls")||(it->toString()=="mkdir")||(it->toString()=="mkfile")||(it->toString()=="cp")||(it->toString()=="mv")||(it->toString()=="rename")||(it->toString()=="rm")||(it->toString()=="history")||(it->toString()=="verbose")||(it->toString()=="exec"))
			cout << i <<'\t'<<it->toString()<<" "<<it->getArgs()<< endl;
		else
			cout << i <<'\t'<<it->toString()<<endl;
		i++;
	}
}


string HistoryCommand::toString(){
	return "history";
}
VerboseCommand::~VerboseCommand() {}

VerboseCommand::VerboseCommand(string args):BaseCommand(args){}
void VerboseCommand::execute(FileSystem & fs){

	if(getArgs()=="0")
		verbose=0;
	else if(getArgs()=="1")
		verbose=1;
	else if(getArgs()=="2")
		verbose=2;
	else if(getArgs()=="3")
		verbose=3;
	else
		cout << "Wrong verbose input" << endl;
}
string VerboseCommand::toString(){return "verbose "+getArgs();}


ErrorCommand::~ErrorCommand() {}
ErrorCommand::ErrorCommand(string args):BaseCommand(args){}
void ErrorCommand::execute(FileSystem & fs){


}
string ErrorCommand::toString(){return getArgs();}

ExecCommand::~ExecCommand() {}
ExecCommand::ExecCommand(string args, const vector<BaseCommand *> & history):BaseCommand(args),history(history){}
void ExecCommand::execute(FileSystem & fs){

	int number = 0;
	string strin=getArgs(); // stoi does not work :( try to convert string to int
	for_each(strin.begin(), strin.end(), [&number](char c){ number = 10 * number + (c - '0');  });
	size_t i=number;
	if (i>=history.size()){
		cout<<"Command not found"<<endl;
		return;
	}
	string st=history.at(i)->toString();
	string tr=history.at(i)->getArgs();

	if(st=="pwd"){
		PwdCommand p=PwdCommand(tr);
		p.execute(fs);}

	if(st=="cd"){
		CdCommand p=CdCommand(tr);
		p.execute(fs);}

	if(st=="ls"){
		LsCommand p=LsCommand(tr);
		p.execute(fs);}

	if(st=="mkdir"){
		MkdirCommand p=MkdirCommand(tr);
		p.execute(fs);}

	if(st=="mkfile"){
		MkfileCommand p=MkfileCommand(tr);
		p.execute(fs);}

	if(st=="cp"){
		CpCommand p=CpCommand(tr);
		p.execute(fs);}

	if(st=="mv"){
		MvCommand p=MvCommand(tr);
		p.execute(fs);}

	if(st=="rename"){
		RenameCommand p=RenameCommand(tr);
		p.execute(fs);}

	if(st=="rm"){
		RmCommand p=RmCommand(tr);
		p.execute(fs);}

	if(st=="history"){
		HistoryCommand p=HistoryCommand(tr,history);
		p.execute(fs);}

	if(st=="verbose"){
		VerboseCommand p=VerboseCommand(tr);
		p.execute(fs);}

	if(st=="exec"){
		ExecCommand p=ExecCommand(tr, history);
		p.execute(fs);}
}
string ExecCommand::toString(){return "exec " + getArgs();}


