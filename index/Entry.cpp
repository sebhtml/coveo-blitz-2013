
#include "Entry.h"
#include <string.h>
#include <stdlib.h>

void Entry::build(int type,const char*content){
	m_type=type;
	strcpy(m_content,content);
	m_length=strlen(m_content);
	m_next=OFFSET_NONE;
}


void Entry::write(char*m_array,uint64_t position){
/*
	uint64_t m_next;
	int m_type;
	int m_length;
	char m_content[128];
*/
	memcpy(m_array+position,&m_next,sizeof(uint64_t));
	position+=sizeof(uint64_t);
	memcpy(m_array+position,&m_type,sizeof(int));
	position+=sizeof(int);
	memcpy(m_array+position,&m_length,sizeof(int));
	position+=sizeof(int);
	memcpy(m_array+position,m_content,m_length*sizeof(char));
	
}

int Entry::getSize(){
	return sizeof(uint64_t)+2*sizeof(int)+m_length*sizeof(char);
}
