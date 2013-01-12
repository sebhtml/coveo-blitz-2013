/**
 * \author Sébastien Boisvert
 */

#include "KorafleWebService.h"
#include "JSONParser.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
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
}

void KorafleWebService::search(const char*uri,const char*query){
	cout<<"{  }"<<endl;
}

void KorafleWebService::push(const char*uri,const char*query){
	cout<<"{\"message\": \"Thank you for your documents\" }"<<endl;

        //char*requestMethod=getenv("REQUEST_METHOD");
        int contentLength=atoi(getenv("CONTENT_LENGTH"));
        
        char*postData=(char*)malloc(contentLength+1);
        size_t values=fread(postData,1,contentLength,stdin);

	cout<<values<<endl;

	JSONParser parser;
	
}
