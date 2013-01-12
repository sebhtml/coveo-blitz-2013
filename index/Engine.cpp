
#include "Engine.h"
#include "Entry.h"


#include <iostream>
using namespace std;

#define OFFSET_MAGIC 0
#define OFFSET_MEMORY 8
#define OFFSET_PREDICATE 16

#define OFFSET_MEMORY_START 24

/**
 * magic 8
 * offset for free
 * predicate->
 * 
 */
void Engine::indexMetaData(const char*subject,const char*predicate,const char*object){

	uint64_t next=getNext();

	if(next==OFFSET_MEMORY_START){

		
		Entry entry;
		entry.build(PREDICATE,predicate);
		int size=entry.getSize();
		setNext(getNext()+size);
		entry.write(m_array,next);
		m_array[OFFSET_PREDICATE]=next;
	}else{
		
	}
}

void Engine::open(){

	m_file="/mnt/block";

	m_mapper.enableReadOperations();
	m_mapper.enableWriteOperations();

	m_array=(char*)m_mapper.mapFile(m_file.c_str());

	cout<<"m_file="<<m_file<<endl;

	m_bytes=m_mapper.getFileSize();
	
	cout<<"Size= "<<m_bytes<<endl;

	reset();
}

void Engine::reset(){
	setNext(OFFSET_MEMORY_START);
}


void Engine::close(){

	m_mapper.unmapFile();
}

uint64_t Engine::getNext(){
	return m_array[OFFSET_MEMORY];
}

void Engine::setNext(uint64_t value){
	m_array[OFFSET_MEMORY]=value;
}
