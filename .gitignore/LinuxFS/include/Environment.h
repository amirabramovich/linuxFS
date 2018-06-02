#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Files.h"
#include "Commands.h"

#include <string>
#include <vector>

using namespace std;

class Environment {
private:
	vector<BaseCommand*> commandsHistory;
	FileSystem fs;
	void clear();
	void copy(const Environment &rhs);

public:
	Environment();
	Environment(const Environment &rhs); // Copy Constructor
	virtual ~Environment();//----------------------Destructor
	Environment & operator=(const Environment &rhs); //Operator =
	Environment(Environment &&rhs); // Move Constructor
	Environment & operator=(Environment &&rhs); // Move Assignment Operator
	void start();
	FileSystem& getFileSystem(); // Get a reference to the file system
	void addToHistory(BaseCommand *command); // Add a new command to the history
	const vector<BaseCommand*>& getHistory() const; // Return a reference to the history of commands
};

#endif
