#include "Engine.h"

#include <string>
#include <iostream>
#include <vector>
using namespace std;

int main(int argc,char**argv){

	const char predicate[]="q";
	const char*object=argv[1];

	Engine engine;
	engine.open();

	vector<string> subjects;
	engine.search(predicate,object,&subjects);

	for(int i=0;i<(int)subjects.size();i++)
		cout<<subjects[i]<<endl;

	engine.close();

	return 0;
}
