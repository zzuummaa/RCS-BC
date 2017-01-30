/*
 * parser.cpp
 *
 *  Created on: 30.01.2017
 *      Author: zzuummaa
 */

#include "parser.h"

#include <string.h>
#include <assert.h>

/**
 * ============================================
 * =               parser                     =
 * ============================================
 */

void parser::setBuffPoint(char* buff) {
	this->buff = buff;
}

char* parser::getBuffPoint() {
	return buff;
}

int parser::cpyBuff(char* buff, int buff_size) {
	int size = getBuffSize();
	assert(buff_size < size);

	memcpy(buff, this->buff, size);

	return size;
}

/**
 * ============================================
 * =              mapParser                   =
 * ============================================
 */

/**
 * Title of data,contained in buff
 */
typedef struct {
	int key;
	int size;
} valTitle;

#define END_KEY -1

mapParser::mapParser(char* buff, int capacity) {
	assert(buff != NULL);
	assert(capacity > 0);

	this->buff = buff + sizeof(mapTitle);
	this->mTitle = (mapTitle*)buff;
	this->capacity = capacity;
}

/**
 * It Finds data with input type
 * return pointer to desired title
 * 	  or  title with key=END_KEY if not found
 */
valTitle* searchTitle(char* buff, int key) {
	valTitle* title = (valTitle*) buff;
	while (title->key != END_KEY) {
		if (title->key == key) {
			break;
		}
		title = (valTitle*)( (char*)title + title->size + sizeof(valTitle) );
	}

	return title;
}

void fillingLastTitle(valTitle* title) {
	title->key  = END_KEY;
	title->size =  0;
}

int mapParser::add(int key, char* val, int size) {
	assert(key >= 0);

	valTitle* title = searchTitle(buff, key);

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

/**
 * return size of data
 *     or -1 if not found
 */
int mapParser::get(int key, char** val) {
	assert(key >= 0);

	valTitle* title = searchTitle(buff, key);

	if (title->key == END_KEY) {
		return -1;
	}

	*val = (char*)title + sizeof(title);

	return title->size;
}

int mapParser::getBuffSize() {
	return mTitle->size;
}

void mapParser::setBuffSize(int size) {
	mTitle->size = size;
}

int markMem(char* buff, int size) {
	memset(buff, '\0', size);

	mapTitle* mtitle = (mapTitle*)buff;
	mtitle->size = sizeof(mapTitle) + sizeof(valTitle);

	fillingLastTitle( (valTitle*)(buff + sizeof(mapTitle)) );

	return 1;
}
