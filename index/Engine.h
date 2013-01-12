
#include "Mapper.h"
#include <stdint.h>
#include "Entry.h"

#include <string>
#include <vector>
using namespace std;

class Engine{
	string m_file;
	Mapper m_mapper;
	char*m_array;
	uint64_t m_bytes;

	uint64_t getHeap();
	void setHeap(uint64_t value);

	uint64_t getNextOffsetForPredicate();

	void addPredicate(uint64_t offset,const char*predicate,uint64_t parentOffset);
	bool fetchPredicate(Entry*entry,const char*predicate,bool write);
	uint64_t findPredicate(const char*predicate);
	void addPredicateInFile(const char*predicate);

	bool fetchObject(Entry*predicateEntry,Entry*entry,const char*object,bool write);
	uint64_t findObject(Entry*predicateEntry,const char*object);
	void addObjectInFile(Entry*predicateEntry,const char*predicate);
	void addObject(uint64_t offset,const char*object);

	bool fetchSubject(Entry*objectEntry,Entry*subjectEntry,const char*subject,bool write);
	uint64_t findSubject(Entry*objectEntry,const char*subject);
	void addSubject(uint64_t offset,const char*subject);
	void addSubjectInFile(Entry*objectEntry,const char*subject);

	void getSubjects(Entry*objectEntry,vector<Entry>*subjects);
public:
	
	void indexMetaData(const char*id,const char*key,const char*value);
	void open();
	void close();
	uint64_t read64Integer(uint64_t offset);
	void write64Integer(uint64_t offset,uint64_t value);

	void reset();

	void search(const char*key,const char*value,vector<string>*subjects);
};
