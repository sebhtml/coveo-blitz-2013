
#include "Entry.h"
#include <string.h>
#include <stdlib.h>

#include <iostream>
using namespace std;

void Entry::build(int type,const char*content){
	m_type=type;
	strcpy(m_content,content);
	m_length=strlen(m_content);
	m_nextRight=OFFSET_NONE;
	m_nextDown=OFFSET_NONE;
}

void Entry::read(const char*m_array,uint64_t position){
	//cout<<"Entry read @"<<position<<endl;
/*
	uint64_t m_next;
	int m_type;
	int m_length;
	char m_content[128];
*/
	m_offset=position;

	memcpy(&m_nextRight,m_array+position,sizeof(uint64_t));
	position+=sizeof(uint64_t);
	memcpy(&m_nextDown,m_array+position,sizeof(uint64_t));
	position+=sizeof(uint64_t);
	memcpy(&m_type,m_array+position,sizeof(int));
	position+=sizeof(int);
	memcpy(&m_length,m_array+position,sizeof(int));
	position+=sizeof(int);
	memcpy(m_content,m_array+position,m_length*sizeof(char));
	
	m_content[m_length]='\0';

}



void Entry::write(char*m_array,uint64_t position){
	//cout<<"Entry write @"<<position<<endl;

/*
	uint64_t m_next;
	int m_type;
	int m_length;
	char m_content[128];
*/
	memcpy(m_array+position,&m_nextRight,sizeof(uint64_t));
	position+=sizeof(uint64_t);
	memcpy(m_array+position,&m_nextDown,sizeof(uint64_t));
	position+=sizeof(uint64_t);
	memcpy(m_array+position,&m_type,sizeof(int));
	position+=sizeof(int);
	memcpy(m_array+position,&m_length,sizeof(int));
	position+=sizeof(int);
	memcpy(m_array+position,m_content,m_length*sizeof(char));
	position+=m_length;

	m_array[position]='\0';
}

int Entry::getSize(){
	return 2*sizeof(uint64_t)+2*sizeof(int)+m_length*sizeof(char);
}


uint64_t Entry::getNextRight(){
	return m_nextRight;
}

uint64_t Entry::getNextDown(){
	return m_nextDown;
}

int Entry::getType(){
	return m_type;
}

int Entry::getLength(){
	return m_length;
}

char*Entry::getContent(){
	return m_content;
}

void Entry::setNextRight(uint64_t value){
	//cout<<"Entry/setNext"<<endl;
	m_nextRight=value;
}

void Entry::setNextDown(uint64_t value){
	//cout<<"Entry/setNext"<<endl;
	m_nextDown=value;
}

uint64_t Entry::getOffset(){

	return m_offset;
}
