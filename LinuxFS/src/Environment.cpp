/*
 * Environment.cpp
 *
 *  Created on: Nov 7, 2017
 *      Author: amir
 */

#include "../include/Environment.h"
#include "../include/GlobalVariables.h"
#include <iostream>

using namespace std;




Environment::Environment():commandsHistory(),fs(){}


Environment::Environment(const Environment &rhs):commandsHistory(),fs(){// Copy Constructor
	if((verbose==1)||(verbose==3))cout << "Environment Environment::Environment(const Environment &rhs)" << endl;
	copy(rhs);
}

void Environment::copy(const Environment &rhs){ //deep copy of rhs Environment
	for (auto it : rhs.commandsHistory)
		commandsHistory.push_back(it);
}

void Environment::clear(){
	//delete &fs;
	for(auto it:commandsHistory)
		delete it;
	commandsHistory.clear();
}

Environment::~Environment(){//Destructor
	if((verbose==1)||(verbose==3))cout << "Environment Environment::~Environment()" << endl;
	clear();
}
Environment& Environment::operator=(const Environment &rhs){//Operator =
	if((verbose==1)||(verbose==3))cout << "Environment Environment& Environment::operator=(const Environment &rhs)" << endl;
	if (this != &rhs){
		commandsHistory.clear();
		fs = rhs.fs;
		for (auto it : rhs.commandsHistory)
			commandsHistory.push_back(it);
	}
	return *this;
}
Environment::Environment(Environment &&rhs):commandsHistory(),fs(){// Move Constructor
	if((verbose==1)||(verbose==3))cout << "Environment Environment::Environment(Environment &&rhs)" << endl;
	commandsHistory.clear();
}
Environment& Environment::operator=(Environment &&rhs){// Move Assignment Operator
	if((verbose==1)||(verbose==3))cout << "Environment Environment& Environment::operator=(Environment &&rhs)" << endl;
	if (this != &rhs){
		commandsHistory.clear();
	    fs = rhs.fs;
	    for (auto it : rhs.commandsHistory){
	            commandsHistory.push_back(it);
	    }
	rhs.commandsHistory.clear();
	}
	return *this;
}




void Environment::start() {
	string input;
	cout << getFileSystem().getWorkingDirectory().getAbsolutePath()<<">";cout.flush();getline(cin,input);
    while(input!="exit"){
    	string str=input.substr (0,input.find_first_of(' ')); //determine what was the command
    	if((verbose==2)||(verbose==3))cout << input << endl;
    	if(str=="pwd"){
    		PwdCommand* p=new PwdCommand("");
    		addToHistory(p);
    		p->execute(getFileSystem());
    	}
    	else if(str=="cd"){

    			CdCommand* c=new CdCommand(input.substr(input.find_first_of(' ')+1));
    			c->execute(getFileSystem());
    			addToHistory(c);

    	}
    	else if(str=="ls"){
    		if(input=="ls"){//to exclude case user wrote "ls" only
    			LsCommand* l=new LsCommand("");
    			l->execute(getFileSystem());
    			addToHistory(l);
    		}
    		else{
    			LsCommand* l=new LsCommand(input.substr(input.find_first_of(' ')+1));
    		    l->execute(getFileSystem());
    		    addToHistory(l);
    		}
    	}
    	else if(str=="mkdir"){

    			MkdirCommand* m=new MkdirCommand(input.substr(input.find_first_of(' ')+1));
    		    m->execute(getFileSystem());
    		    addToHistory(m);

    	}
    	else if(str=="mkfile"){

    			MkfileCommand* m=new MkfileCommand(input.substr(input.find_first_of(' ')+1));
    			m->execute(getFileSystem());
    			addToHistory(m);

    	}
    	else if(str=="cp"){

    			CpCommand* c=new CpCommand(input.substr(input.find_first_of(' ')+1));
    			c->execute(getFileSystem());
    			addToHistory(c);

    	}
    	else if(str=="mv"){

    			MvCommand* m=new MvCommand(input.substr(input.find_first_of(' ')+1));
    			m->execute(getFileSystem());
    			addToHistory(m);

    	}
    	else if(str=="rename"){

    			RenameCommand* r=new RenameCommand(input.substr(input.find_first_of(' ')+1));
    			r->execute(getFileSystem());
    			addToHistory(r);

    	}
    	else if(str=="rm"){

    			RmCommand* r=new RmCommand(input.substr(input.find_first_of(' ')+1));
    			r->execute(getFileSystem());
    			addToHistory(r);

    	}
    	else if(str=="history"){
    		HistoryCommand* h=new HistoryCommand("",getHistory());
    		h->execute(getFileSystem());
    		addToHistory(h);
    	}
    	else if(str=="verbose"){
    		VerboseCommand* v=new VerboseCommand(input.substr(input.find_first_of(' ')+1));
    		v->execute(getFileSystem());
    		addToHistory(v);
    	}
    	else if(str=="exec"){
    		ExecCommand* e=new ExecCommand(input.substr(input.find_first_of(' ')+1),getHistory());
    		e->execute(getFileSystem());
    		addToHistory(e);
    	}
    	else{cout <<str+": Unknown command" << endl;
    	ErrorCommand* e=new ErrorCommand(str);
    	e->execute(getFileSystem());
    	addToHistory(e);
    	}
    	cout << getFileSystem().getWorkingDirectory().getAbsolutePath()<<">";cout.flush();getline(cin,input);
    }
}

FileSystem& Environment::getFileSystem(){// Get a reference to the file system
	return fs;
}
void Environment::addToHistory(BaseCommand *command){// Add a new command to the history
	commandsHistory.push_back(command);
}
const vector<BaseCommand*>& Environment::getHistory() const{// Return a reference to the history of commands
	return commandsHistory;
}

