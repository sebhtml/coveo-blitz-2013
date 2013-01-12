
#include "Mapper.h"
#include <stdint.h>

#include <string>
using namespace std;

class Engine{
	string m_file;
	Mapper m_mapper;
	char*m_array;
	uint64_t m_bytes;

	uint64_t getNext();
	void setNext(uint64_t value);
	void reset();

	uint64_t getNextOffsetForPredicate();

public:
	
	void indexMetaData(const char*id,const char*key,const char*value);
	void open();
	void close();

};
