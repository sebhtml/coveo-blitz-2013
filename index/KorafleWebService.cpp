/**
 * \author Sébastien Boisvert
 */

#include "KorafleWebService.h"
#include "Engine.h"

#include <jsoncpp/json/json.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void KorafleWebService::processQuery(const char*uri,const char*query){

/*
http://ec2-50-17-15-66.compute-1.amazonaws.com:11111/Korafle.cgi/search?q=Salut

GET /Korafle.cgi/search?q=Salut HTTP/1.1
or
/Korafle.cgi/push HTTP/1.1
*/

	cout<<"X-Powered-By: Les Jiraffes"<<endl;
	cout<<"Access-Control-Allow-Origin: *"<<endl;
	cout<<"Content-type: application/json\n\n";


#if 0
	cout<<"{"<<endl;
	cout<<"\"REQUEST_URI\":"<<"\""<<uri<<"\","<<endl;
	cout<<"\"QUERY_STRING\":"<<"\""<<query<<"\","<<endl;
#endif

// /Korafle.cgi/search
// /Korafle.cgi/push
// /Korafle.cgi/reset

	int operationPosition=13;

	if(operationPosition>=(int)strlen(uri))
		return;

	char operationCode=uri[operationPosition];
	
#if 0
	cout<<"\"operationCode\":"<<"\""<<operationCode<<"\""<<endl;


	cout<<"}"<<endl;
#endif

	if(operationCode=='s')
		search(uri,query);
	else if(operationCode=='p')
		push(uri,query);
	else if(operationCode=='r')
		reset(uri,query);
}

#include <map>

bool KorafleWebService::getValue(const char*query,const char*name,char*value,int maximumValueLength){
	for(int i=0;i<(int)strlen(query);i++){
		bool match=true;

		//cout<<"Query+i: "<<query+i<<endl;
		//cout<<"Name:    "<<name<<endl;

		for(int j=0;j<(int)strlen(name);j++){
			if(query[i+j]!=name[j]){
				match=false;
				break;
			}
		}

		//cout<<"Match: "<<match<<endl;

		if(match){
			int startingPosition=i+strlen(name)+1;
			int endingPosition=startingPosition;
			while(endingPosition<(int)strlen(query)){
				if(query[endingPosition]=='&')
					break;
				endingPosition++;
			}
			int count=endingPosition-startingPosition;

/* somebody is trying to break in */

			if(count>maximumValueLength)
				return false;

			//cout<<"Value= "<<query+startingPosition<<endl;
			//cout<<"Count= "<<count<<endl;

			memcpy(value,query+startingPosition,count);
			value[count]='\0';

			return true;
		}
	}
	
	return false;
}


void KorafleWebService::search(const char*uri,const char*query){
	
	Engine engine;
	engine.open();

	char qQuery[128];

	getValue(query,"q",qQuery,128);
	engine.search("q",qQuery);
	engine.close();

	int NB_FACETS = 3;
	int NB_KEYS   = 4;
	int NB_DOC 	  = 10;

	cout<<"{";

	cout << "\"facets\":{";

	for (int i = 0; i < NB_FACETS; ++i)
	{
		if (i > 0) cout << ",";
		cout << "\"FACET_" << i << "_NAME\": {";

		for (int j = 0; j < NB_KEYS; ++j)
		{
			if (j > 0) cout << ",";
			cout << "\"KEY_" << j << "_NAME\":" <<  NB_DOC ;
		}

		cout << "}";
	}

	int NB_RESULTS = 15;

	cout << "}, \"results\": [ ";

	for (int i = 0; i < NB_RESULTS; ++i)
	{
		int DOC_ID = i; //!!! A changer!
		string DOC_TEXT = "Document description";
		if (i > 0) cout << ",";
		cout << "{\"id\":" << DOC_ID << ",\"text\":\"" << DOC_TEXT << "\"}";
	}

	cout << "]";

	cout << "}"<<endl;
}

/**
 * \see http://jsoncpp.sourceforge.net/
 */
void KorafleWebService::push(const char*uri,const char*query){

	cout<<"{\"message\": \"Thank you for your documents\" }"<<endl;

        //char*requestMethod=getenv("REQUEST_METHOD");
        int contentLength=atoi(getenv("CONTENT_LENGTH"));
        
        char*postData=(char*)malloc(contentLength+1);
        size_t values=fread(postData,1,contentLength,stdin);

	values++;

	postData[contentLength]='\0';

/*
	ofstream joe("/tmp/joe");
	joe<<postData;
	joe.close();
*/

	Engine engine;
	engine.open();

	//cout<<postData<<endl;

	Json::Reader reader;
	Json::Value root;   // will contains the root value after parsing.
	
	reader.parse(postData,postData+contentLength-1,root,false);

	string id="NULL";
	bool gotIdentifier=false;

	for(Json::ValueIterator i=root.begin();i!=root.end();i++){
		string key=i.key().asString();
		//cout<<key<<endl;

		Json::Value value=*i;

		if(key=="id"){
			id=value.asString();
			gotIdentifier=true;
			break;
		}
	}

	if(!gotIdentifier)
		return;

	string qOperation="q";


	for(Json::ValueIterator i=root.begin();i!=root.end();i++){
		string key=i.key().asString();
		//cout<<key<<endl;

		Json::Value value=*i;

		if(key=="text"){
			vector<string> keyWords;
			string content=value.asString();
			tokenize(&content,&keyWords);

			for(int i=0;i<(int)keyWords.size();i++)
				engine.indexMetaData(id.c_str(),qOperation.c_str(),keyWords[i].c_str());
		}else{

			const char*keyForIndexing=key.c_str();
			for(Json::ValueIterator j=value.begin();j!=value.end();j++){
				string word=(*j).asString();

				engine.indexMetaData(id.c_str(),keyForIndexing,word.c_str());
			}

		}

#if DEBUG1
		if(key=="id")
			cout<<"id="<<id<<endl;

		if(value.isString()){
			string content=value.asString();
			cout<<content<<endl;

		}else if(value.isArray()){
			for(Json::ValueIterator j=value.begin();j!=value.end();j++){
				cout<<","<<(*j).asString();
			}
			cout<<endl;
		}
#endif
	}

	engine.close();
	
	//node->debug();

}

void KorafleWebService::tokenize(string*content,vector<string>*array){

	int first=0;
	int current=first;
	int maximum=content->length()-1;

	while(1){
		if(isWhiteSpace((*content)[current])){
			
			string word=content->substr(first,current-1-first+1);
			array->push_back(word);

			//cout<<" word="<<word<<endl;
			//int nextWhiteSpace=getNextWhiteSpace(content->c_str(),current);

			int lastWhiteSpace=getLastWhiteSpace(content->c_str(),current);

			first=lastWhiteSpace+1;
			current=first;

		}

		current++;

		if(current==maximum)
			break;
	}
	
}

int KorafleWebService::getNextWhiteSpace(const char*content,int current){
	int l=strlen(content);

	while(current<l&&!isWhiteSpace(content[current]))
		current++;

	return current;
}

int KorafleWebService::getLastWhiteSpace(const char*content,int current){
	int l=strlen(content);

	while(current<l&&isWhiteSpace(content[current]))
		current++;

	if(!isWhiteSpace(content[current]))
		current--;

	return current;
}

bool KorafleWebService::isWhiteSpace(char symbol){
	return (symbol==' '
		||symbol=='\t'
		||symbol=='\n'
		||symbol=='<'
		||symbol=='>'
		||symbol=='"'
		||symbol=='\''
		||symbol==','
		||symbol=='.'
		||symbol=='('
		||symbol=='|'
		||symbol=='/'
		||symbol==')'
		);
}

void KorafleWebService::reset(const char*uri,const char*query){
	cout<<"Reset"<<endl;
	Engine engine;
	engine.open();
	engine.reset();
	engine.close();
}
