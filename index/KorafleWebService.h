
#include <string>
#include <vector>
using namespace std;

/**
 * \author Sébastien Boisvert
 */
class KorafleWebService{

	void search(const char*uri,const char*query);
	void push(const char*uri,const char*query);
	void reset(const char*uri,const char*query);

	void tokenize(string*content,vector<string>*array);

	bool isWhiteSpace(char symbol);
	int getNextWhiteSpace(const char*content,int current);
	int getLastWhiteSpace(const char*content,int current);
	bool getValue(const char*query,const char*name,char*value,int maximumValueLength);
public:
	void processQuery(const char*uri,const char*query);
};
