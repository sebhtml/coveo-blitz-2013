
#define SUBJECT 0
#define PREDICATE 1
#define OBJECT 3

#define OFFSET_NONE 0

#include <stdint.h>

class Entry{

	uint64_t m_next;
	int m_type;
	int m_length;
	char m_content[128];
public:

	void write(char*m_array,uint64_t position);
	void build(int type,const char*content);
	int getSize();

};