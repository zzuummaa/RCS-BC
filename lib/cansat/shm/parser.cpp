/*
 * parser.cpp
 *
 *  Created on: 30.01.2017
 *      Author: zzuummaa
 */

#include "parser.h"

#include <string.h>
#include <assert.h>
#include <time.h>

/**
 * ============================================
 * =               parser                     =
 * ============================================
 */

void initParser(parser* thism, char* buff, int capacity) {
	assert(buff != NULL);
	assert(capacity > 0);

	thism->setBuffPoint(buff);
	thism->capacity = capacity;
}

void parser::setBuffPoint(char* buff) {
	this->firstTitle = (valTitle*)(buff + sizeof(memoryTitle));
	this->mTitle = (memoryTitle*)buff;
}

char* parser::getBuffPoint() {
	return (char*)mTitle;
}

int parser::cpyBuff(char* buff, int buff_size) {
	int size = getBuffSize();
	assert(buff_size < size);

	memcpy(buff, getBuffPoint(), size);

	return size;
}

int parser::getBuffSize() {
	return mTitle->size;
}

void parser::setBuffSize(int size) {
	mTitle->size = size;
}

void parser::setCapacity(int capacity) {
	this->capacity = capacity;
}

void fillingLastTitle(valTitle* title) {
	title->key  = END_KEY;
	title->size =  0;
}

int parser::addVal(valTitle* title, const int key, const char* val, const int size) {
	int buffSize = getBuffSize();
	assert(buffSize != -1);

	//If record with key not found
	if (title->key == END_KEY) {
		int newSize = buffSize + size + sizeof(valTitle);
		assert(newSize <= capacity);
		setBuffSize(newSize);

		valTitle* newLastTitle = (valTitle*)( (char*)title + sizeof(valTitle) + size );
		fillingLastTitle(newLastTitle);

		title->key = key;
		title->size = size;
	}

	if (title->size != size) {
		return 0;
	}

	memcpy((char*)title + sizeof(valTitle), val, size);

	return 1;
}

char* parser::getVal(valTitle* title) {
	return (char*)title + sizeof(title);
}

/**
 * ============================================
 * =              mapParser                   =
 * ============================================
 */

mapParser::mapParser() {
}

mapParser::mapParser(char* buff, int capacity) {
	initParser(this, buff, capacity);
}

valTitle* nextTitle(valTitle* title) {
	return (valTitle*)( (char*)title + title->size + sizeof(valTitle) );
}

/**
 * It Finds data with input type
 * return pointer to desired title
 * 	  or  title with key=END_KEY if not found
 */
valTitle* searchTitle(valTitle* firstTitle, int key) {
	valTitle* title = firstTitle;
	while (title->key != END_KEY) {
		if (title->key == key) {
			break;
		}
		title = nextTitle(title);
	}

	return title;
}

int mapParser::add(int key, const char* val, const int size) {
	assert(key >= 0);

	valTitle* title = searchTitle(firstTitle, key);

	return addVal(title, key, val, size);
}

/**
 * return size of data
 *     or -1 if not found
 */
int mapParser::get(int key, char** val) {
	assert(key >= 0);

	valTitle* title = searchTitle(firstTitle, key);

	if (title->key == END_KEY) {
		return -1;
	}

	*val = getVal(title);

	return title->size;
}

int mapParser::getKeyMap(map<int, char*>* m) {
	valTitle* title = firstTitle;

	while (title->key != END_KEY) {
		m->insert(pair<int, char*>( title->key, getVal(title) ));
		title = nextTitle(title);
	}

	return m->size();
}

int markMem(char* buff, int size) {
	assert(size >= sizeof(memoryTitle) + sizeof(valTitle) );

	memset(buff, '\0', size);

	memoryTitle* mtitle = (memoryTitle*)buff;
	mtitle->size = sizeof(memoryTitle) + sizeof(valTitle);

	fillingLastTitle( (valTitle*)(buff + sizeof(memoryTitle)) );

	return 1;
}


/**
 * ============================================
 * =          multiMapParser                  =
 * ============================================
 */

multiMapParser::multiMapParser(char* buff, int capacity) {
	initParser(this, buff, capacity);
}

int multiMapParser::add(const int key, const char* val, const int valSize) {
	assert(key >= 0);

	valTitle* title = searchTitle(firstTitle, END_KEY);

	return addVal(title, key, val, valSize);
}

int multiMapParser::get(int key, vector<char*>* vals) {
	assert(key >= 0);

	vals->clear();
	valTitle* title = firstTitle;

	for (;;) {
		title = searchTitle(title, capacity);

		if (title->key == END_KEY) break;

		vals->push_back( getVal(title) );
	}

	return vals->size();
}

/**
 * ============================================
 * =            timeParser                    =
 * ============================================
 */

timeParser::timeParser() {
	latestLastTitle = NULL;
}

timeParser::timeParser(char* buff, int capacity) {
	initParser(this, buff, capacity);
}

void timeParser::setBuffPoint(char* buff) {
	parser::setBuffPoint(buff);
	latestLastTitle = firstTitle;
}

int timeParser::add(int key, const char* val, const int valSize) {
	assert(key >= 0);

	//Getting time in seconds from 1970 year.
	//Default time_t type is long int, but we use int,
	//It's bullshit solution, but it will crash maybe in 2027 year.
	int curTime = (int)time(0);

	valTitle* title = searchTitle(latestLastTitle, curTime);
	latestLastTitle = title;

	if ( title->key == END_KEY) {
		char tmp[1];
		if ( addVal(title, curTime, tmp, 0) == 0) {
			return 0;
		}
	}

	char* mem = getVal(title);
	multiMapParser mmparser(mem, capacity - (mem - (char*)mTitle));

	return mmparser.add(key, val, valSize);
}



int timeParser::getTimeMap(map<int, char*>* m) {
	return mapParser::getKeyMap(m);
}

int timeParser::getFromTime(int time, multiMapParser* mmparser) {
	assert(time >= 0);

	valTitle* title = searchTitle(firstTitle, capacity);

	if ( title->key == END_KEY) {
		return 0;
	}

	mmparser->setBuffPoint( getVal(title) );
	mmparser->setBuffSize( title->size );
	mmparser->setCapacity( title->size );

	return 1;
}
