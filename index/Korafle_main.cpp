
#include "KorafleWebService.h"

#include <stdlib.h>

/**
 * \author Sébastien Boisvert
 */
int main(int argc,char**argv){

	const char*requestUri=getenv("REQUEST_URI");
	const char*queryString=getenv("QUERY_STRING");
	KorafleWebService service;
	service.processQuery(requestUri,queryString);

	return 0;
}
