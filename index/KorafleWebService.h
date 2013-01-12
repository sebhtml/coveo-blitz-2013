
/**
 * \author Sébastien Boisvert
 */
class KorafleWebService{

	void search(const char*uri,const char*query);
	void push(const char*uri,const char*query);
public:
	void processQuery(const char*uri,const char*query);
};
