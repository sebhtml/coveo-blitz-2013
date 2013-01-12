/**
 * \author Sébastien Boisvert
 */

#include "KorafleWebService.h"

#include <iostream>
using namespace std;

void KorafleWebService::processQuery(const char*query){
	cout<<"X-Powered-By: Les Jiraffes"<<endl;
	cout<<"Access-Control-Allow-Origin: *"<<endl;
	cout<<("Content-type: application/json\n\n");

	cout<<"Les jiraffes"<<endl;

}

