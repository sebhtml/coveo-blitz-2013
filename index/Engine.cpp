
#include "Engine.h"
#include "Entry.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <vector>
#include <iostream>
using namespace std;

#define OFFSET_MAGIC 0
#define OFFSET_BUSY 8
#define OFFSET_MEMORY_HEAP 16
#define OFFSET_PREDICATE_FIRST 24
#define OFFSET_PREDICATE_LAST 32
#define OFFSET_MEMORY_START 40

#define BUSY_NO 0
#define BUSY_YES 1
#define BUSY_DELAY 10000


/*

	TODO: hash with sha1 objects
*/
/**
 * magic 8
 * offset for free
 * predicate->
 * 
 */
void Engine::indexMetaData(const char*subject,const char*predicate,const char*object){

	while(read64Integer(OFFSET_BUSY)==BUSY_YES){
		usleep(BUSY_DELAY);
	}

	write64Integer(OFFSET_BUSY,BUSY_YES);
			
	cout<<endl;
	cout<<"indexMetaData Subject= "<<subject;
	cout<<" Predicate= "<<predicate;
	cout<<" Object= "<<object;
	cout<<endl;

	Entry entry;
	fetchPredicate(&entry,predicate,true);

	Entry objectEntry;
	fetchObject(&entry,&objectEntry,object,true);

	Entry  subjectEntry;
	fetchSubject(&objectEntry,&subjectEntry,subject,true);

	cout<<"Subject offset: "<<subjectEntry.getOffset()<<endl;

	write64Integer(OFFSET_BUSY,BUSY_NO);
}

uint64_t Engine::findSubject(Entry*objectEntry,const char*subject){
	//cout<<"findObject "<<object<<endl;

	//bool debug=strcmp("name",predicate)==0;


	uint64_t offset=objectEntry->getNextDown();

	if(offset==OFFSET_NONE){
		//if(debug)
			//cout<<"returns OFFSET_NONE"<<endl;
		return OFFSET_NONE;
	}

	//cout<<"will search"<<endl;

	while(offset!=OFFSET_NONE){
		
		Entry entry;
		//cout<<"loading @"<<offset<<endl;
		entry.read(m_array,offset);
		//cout<<"done loading"<<endl;
		//cout<<entry.getContent()<<endl;

		if(strcmp(entry.getContent(),subject)==0){
			//cout<<"FOUND object"<<object<<" @"<<offset<<endl;
			return offset;
		}


		//cout<<"getting next"<<endl;

		//uint64_t lastGood=offset;
		offset=entry.getNextRight();

/*
		if(debug)
	
*/		//

	
		//if(debug)
			//cout<<"Current "<<lastGood<<" Next "<<offset<<endl;
	}

	return OFFSET_NONE;
}

uint64_t Engine::findObject(Entry*predicateEntry,const char*object){
	cout<<"findObject "<<object<<endl;

	//bool debug=strcmp("name",predicate)==0;


	uint64_t offset=predicateEntry->getNextDown();

	if(offset==OFFSET_NONE){
		//if(debug)
			//cout<<"returns OFFSET_NONE"<<endl;
		return OFFSET_NONE;
	}

	//cout<<"will search"<<endl;

	while(offset!=OFFSET_NONE){
		
		Entry entry;
		//cout<<"loading @"<<offset<<endl;
		entry.read(m_array,offset);
		//cout<<"done loading"<<endl;
		//cout<<entry.getContent()<<endl;

		if(strcmp(entry.getContent(),object)==0){
			//cout<<"FOUND object"<<object<<" @"<<offset<<endl;
			return offset;
		}


		//cout<<"getting next"<<endl;

		//uint64_t lastGood=offset;
		offset=entry.getNextRight();

/*
		if(debug)
	
*/		//

	
		//if(debug)
			//cout<<"Current "<<lastGood<<" Next "<<offset<<endl;
	}

	return OFFSET_NONE;
}



uint64_t Engine::findPredicate(const char*predicate){
	//cout<<"findPredicate "<<predicate<<endl;

	//bool debug=strcmp("name",predicate)==0;


	uint64_t offset=read64Integer(OFFSET_PREDICATE_FIRST);

	if(offset==OFFSET_NONE){
		//if(debug)
			//cout<<"returns OFFSET_NONE"<<endl;
		return OFFSET_NONE;
	}

	while(offset!=OFFSET_NONE){
		
		Entry entry;
		entry.read(m_array,offset);

		if(strcmp(entry.getContent(),predicate)==0){
			//cout<<"FOUND predicate "<<predicate<<endl;
			return offset;
		}


		//uint64_t lastGood=offset;
		offset=entry.getNextRight();

/*
		if(debug)
	
*/		//cout<<entry.getContent()<<endl;
	
		//if(debug)
			//cout<<"Current "<<lastGood<<" Next "<<offset<<endl;
	}

	return OFFSET_NONE;
}

bool Engine::fetchSubject(Entry*objectEntry,Entry*subjectEntry,const char*subject,bool write){

	//cout<<"[fetchObject"<<endl;

// already have it
	uint64_t subjectOffset=findSubject(objectEntry,subject);

	if(subjectOffset!=OFFSET_NONE){
		//cout<<"Using copy found"<<endl;
		subjectEntry->read(m_array,subjectOffset);
		return true;
	}

	if(!write)
		return false;
// add it

	addSubjectInFile(objectEntry,subject);

// search it again
	return fetchObject(objectEntry,subjectEntry,subject,write);

	
}

bool Engine::fetchObject(Entry*predicateEntry,Entry*entry,const char*object,bool write){

	//cout<<"[fetchObject"<<endl;

// already have it
	uint64_t objectOffset=findObject(predicateEntry,object);

	if(objectOffset!=OFFSET_NONE){
		//cout<<"Using copy found"<<endl;
		entry->read(m_array,objectOffset);
		return true;
	}

	if(!write)
		return false;

// add it

	addObjectInFile(predicateEntry,object);

// search it again
	return fetchObject(predicateEntry,entry,object,write);

	
}

bool Engine::fetchPredicate(Entry*entry,const char*predicate,bool write){

// already have it
	uint64_t predicateOffset=findPredicate(predicate);
	if(predicateOffset!=OFFSET_NONE){
		//cout<<"Using copy found"<<endl;
		entry->read(m_array,predicateOffset);
		return true;
	}

	if(!write)
		return false;
// add it

	addPredicateInFile(predicate);

// search it again
	return fetchPredicate(entry,predicate,write);
}

void Engine::addSubjectInFile(Entry*objectEntry,const char*subject){

	cout<<"[addSubjectInFile]"<<endl;

	if(objectEntry->getNextDown()==OFFSET_NONE){
		
		//cout<<"first"<<endl;

		uint64_t contentOffset=getHeap();

		//cout<<"will use offset "<<contentOffset<<endl;
		addSubject(contentOffset,subject);

		objectEntry->setNextDown(contentOffset);
		objectEntry->write(m_array,objectEntry->getOffset());

		//cout<<"done"<<endl;
	}else{

		//cout<<"not first"<<endl;
		uint64_t contentOffset=getHeap();
		//cout<<"OFFSET_PREDICATE_LAST "<<read64Integer(OFFSET_PREDICATE_LAST)<<endl;

		addSubject(contentOffset,subject);

		uint64_t offset=objectEntry->getNextDown();

		while(1){
			Entry entry;
			entry.read(m_array,offset);
			offset=entry.getNextRight();

			if(offset==OFFSET_NONE){
				entry.setNextRight(contentOffset);
				entry.write(m_array,entry.getOffset());
				break;
			}
		}
	}
}



void Engine::addObjectInFile(Entry*predicateEntry,const char*object){

	cout<<"[addObjectInFile]"<<endl;

	if(predicateEntry->getNextDown()==OFFSET_NONE){
		
		//cout<<"first"<<endl;

		uint64_t contentOffset=getHeap();

		//cout<<"will use offset "<<contentOffset<<endl;
		addObject(contentOffset,object);

		predicateEntry->setNextDown(contentOffset);
		predicateEntry->write(m_array,predicateEntry->getOffset());

		//cout<<"done"<<endl;
	}else{

		//cout<<"not first"<<endl;
		uint64_t contentOffset=getHeap();
		//cout<<"OFFSET_PREDICATE_LAST "<<read64Integer(OFFSET_PREDICATE_LAST)<<endl;

		addObject(contentOffset,object);

		uint64_t offset=predicateEntry->getNextDown();
		while(1){
			Entry entry;
			entry.read(m_array,offset);
			offset=entry.getNextRight();

			if(offset==OFFSET_NONE){
				entry.setNextRight(contentOffset);
				entry.write(m_array,entry.getOffset());
				break;
			}
		}
	}
}

void Engine::addPredicateInFile(const char*predicate){
	if(read64Integer(OFFSET_PREDICATE_FIRST)==OFFSET_NONE){
		
		//cout<<"first"<<endl;

		uint64_t contentOffset=getHeap();

		//cout<<"will use offset "<<contentOffset<<endl;
		addPredicate(contentOffset,predicate,OFFSET_NONE);

		write64Integer(OFFSET_PREDICATE_FIRST,contentOffset);
	}else{

		//cout<<"not first"<<endl;
		uint64_t contentOffset=getHeap();
		//cout<<"OFFSET_PREDICATE_LAST "<<read64Integer(OFFSET_PREDICATE_LAST)<<endl;

		addPredicate(contentOffset,predicate,read64Integer(OFFSET_PREDICATE_LAST));

	}
}

void Engine::open(){

	m_file="/mnt/block";

	m_mapper.enableReadOperations();
	m_mapper.enableWriteOperations();

	m_array=(char*)m_mapper.mapFile(m_file.c_str());

	//cout<<"m_file="<<m_file<<endl;

	m_bytes=m_mapper.getFileSize();
	
	//cout<<"Size= "<<m_bytes<<endl;

	//reset();
}

void Engine::reset(){
	write64Integer(OFFSET_PREDICATE_FIRST,OFFSET_NONE);
	write64Integer(OFFSET_BUSY,BUSY_NO);
	write64Integer(OFFSET_MEMORY_HEAP,OFFSET_MEMORY_START);
}


void Engine::close(){

	m_mapper.unmapFile();
}

uint64_t Engine::getHeap(){

	uint64_t offset=read64Integer(OFFSET_MEMORY_HEAP);
	if(offset>=m_bytes)
		cout<<"Invalid offset in getHeap from OFFSET_MEMORY_HEAP"<<endl;

	return offset;
}

void Engine::setHeap(uint64_t value){
	if(value>=m_bytes)
		cout<<"Error invalid value in setHeap"<<endl;

	//cout<<"Setting OFFSET_MEMORY_HEAP to "<<value<<endl;
	write64Integer(OFFSET_MEMORY_HEAP,value);
}

/* not used */
uint64_t Engine::getNextOffsetForPredicate(){

	//cout<<"[getNextOffsetForPredicate"<<endl;


	//return OFFSET_PREDICATE_LAST;
//m_array[OFFSET_PREDICATE_FIRST]==OFFSET_NONE

	uint64_t offset=read64Integer(OFFSET_PREDICATE_FIRST);

	if(offset==OFFSET_NONE){
		return read64Integer(OFFSET_MEMORY_HEAP);
	}

	uint64_t lastGood=OFFSET_NONE;
	while(offset!=OFFSET_NONE){
		
		Entry entry;
		entry.read(m_array,offset);

		lastGood=offset;

		offset=entry.getNextRight();
	
		//cout<<"Current "<<lastGood<<" Next "<<offset<<endl;
	}

	return lastGood;
}

void Engine::addSubject(uint64_t offset,const char*subject){
	//cout<<endl;
	cout<<"addSubject "<<subject<<" offset="<<offset<<endl;

	if(!(offset<m_bytes)){
		cout<<"Invalid offset"<<endl;
		return;
	}

	Entry entry;
	entry.build(SUBJECT,subject);
	entry.write(m_array,offset);

	uint64_t newAddress=read64Integer(OFFSET_MEMORY_HEAP)+entry.getSize();
	setHeap(newAddress);

	cout<<"addSubject entry saved."<<endl;


}

void Engine::addObject(uint64_t offset,const char*object){
	//cout<<endl;
	cout<<"addObject "<<object<<" offset="<<offset<<endl;

	if(!(offset<m_bytes)){
		cout<<"Invalid offset"<<endl;
		return;
	}

	Entry entry;
	entry.build(OBJECT,object);
	entry.write(m_array,offset);

	uint64_t newAddress=read64Integer(OFFSET_MEMORY_HEAP)+entry.getSize();
	setHeap(newAddress);

	cout<<"addObject entry saved."<<endl;


}

void Engine::addPredicate(uint64_t offset,const char*predicate,uint64_t parentOffset){
	//cout<<endl;
	cout<<"addPredicate "<<predicate<<" offset="<<offset<<" parentOffset="<<parentOffset<<endl;

	if(!(offset<m_bytes)){
		cout<<"Invalid offset"<<endl;
		return;
	}

	Entry entry;
	entry.build(PREDICATE,predicate);
	entry.write(m_array,offset);
	write64Integer(OFFSET_PREDICATE_LAST,offset);

	uint64_t newAddress=read64Integer(OFFSET_MEMORY_HEAP)+entry.getSize();
	setHeap(newAddress);

	//cout<<"addPredicate entry saved."<<endl;


// update the next of the parent
	if(parentOffset!=OFFSET_NONE){

		cout<<"updating next of parent @"<<parentOffset<<endl;

		Entry parentEntry;
		parentEntry.read(m_array,parentOffset);
		parentEntry.setNextRight(offset);
		
		//cout<<"saving parent"<<endl;
		parentEntry.write(m_array,parentOffset);
	}

	//cout<<"completed addPredicate offset="<<offset<<endl;

}

uint64_t Engine::read64Integer(uint64_t offset){
	uint64_t value;
	memcpy(&value,m_array+offset,sizeof(uint64_t));
	return value;
}

void Engine::write64Integer(uint64_t offset,uint64_t value){

	memcpy(m_array+offset,&value,sizeof(uint64_t));
}

void Engine::search(const char*predicate,const char*object,vector<string>*subjects){

	//cout<<"[Engine::search]"<<endl;

	Entry predicateEntry;
	if(!fetchPredicate(&predicateEntry,predicate,false)){
		//cout<<"No Predicate found!"<<endl;
		return;
	}

	//cout<<"Predicate: "<<predicateEntry.getContent()<<endl;

	Entry objectEntry;
	if(!fetchObject(&predicateEntry,&objectEntry,object,false)){
		//cout<<"No object found!"<<endl;
		return;
	}

	vector<Entry> subjectEntries;
	//cout<<"Object: "<<objectEntry.getContent()<<endl;

	getSubjects(&objectEntry,&subjectEntries);

	for(int i=0;i<(int)subjectEntries.size();i++){
		subjects->push_back(subjectEntries[i].getContent());
	}
}

void Engine::getSubjects(Entry*objectEntry,vector<Entry>*subjects){
	
	//cout<<"getSubjects"<<endl;
	uint64_t offset=objectEntry->getNextDown();

	while(offset!=OFFSET_NONE){
		
		Entry entry;
		entry.read(m_array,offset);
		offset=entry.getNextRight();

		if(entry.getType()!=SUBJECT)
			cout<<"invalid type"<<endl;

		subjects->push_back(entry);
	}
}

