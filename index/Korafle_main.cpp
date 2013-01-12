
#include "KorafleWebService.h"

#include <stdlib.h>

/**
 * \author Sébastien Boisvert
 */
int main(int argc,char**argv){

	const char*queryString=getenv("QUERY_STRING");
	KorafleWebService service;
	service.processQuery(queryString);

	return 0;
}
