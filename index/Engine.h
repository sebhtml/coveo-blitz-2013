
#include "Mapper.h"
#include <stdint.h>

#include <string>
using namespace std;

class Engine{
	string m_file;
	Mapper m_mapper;
	char*m_array;
	uint64_t m_bytes;

	uint64_t getHeap();
	void setHeap(uint64_t value);
	void reset();

	uint64_t getNextOffsetForPredicate();

	void addPredicate(uint64_t offset,const char*predicate,uint64_t parentOffset);
public:
	
	void indexMetaData(const char*id,const char*key,const char*value);
	void open();
	void close();
	uint64_t read64Integer(uint64_t offset);
	void write64Integer(uint64_t offset,uint64_t value);

};
