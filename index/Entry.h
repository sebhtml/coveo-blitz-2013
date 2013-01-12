
#ifndef _Entry
#define _Entry

#define SUBJECT 0
#define PREDICATE 1
#define OBJECT 3

#define OFFSET_NONE 0

#include <stdint.h>

class Entry{

	uint64_t m_offset;

	uint64_t m_nextRight;
	uint64_t m_nextDown;
	int m_type;
	int m_length;
	char m_content[128];
public:

	void write(char*m_array,uint64_t position);
	void read(const char*m_array,uint64_t position);
	void build(int type,const char*content);
	int getSize();

	uint64_t getNextRight();
	uint64_t getNextDown();
	int getType();
	int getLength();
	char*getContent();
	void setNextRight(uint64_t value);
	void setNextDown(uint64_t value);

	uint64_t getOffset();
};

#endif
