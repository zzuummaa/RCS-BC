/*
 * parser.h
 *
 *  Created on: 30.01.2017
 *      Author: zzuummaa
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <vector>
#include <map>

using namespace std;

/**
 * Description:
 *
 * Memory organization ( I suppose:) )
 *
 * [memoryTitle][valTitle 1][  data1  ][valTitle 2][  data2   ]..[valTitle {lastTitNum}]
 *
 * In title with number "lastTitNum", field "type" is equals END_KEY (type=END_KEY).
 */

/**
 * Title of data,contained in buff
 *
 * "size" - size in bytes of all memory which are in use in current moment
 */
typedef struct {
	int key;
	int size;
} valTitle;

#define END_KEY -1

typedef struct {
	int size;
} memoryTitle;

class parser {
protected:
	memoryTitle* mTitle;
	valTitle* firstTitle;
	int capacity;

	char* getVal(valTitle* title);
	int addVal(valTitle* title, const int key, const char* val, const int size);
public:
	friend void initParser(parser* thism, char* buff, int capacity);

	virtual ~parser() {};

	/**
	 * return size of data, contained in buff
	 *     or -1 if error occurred
	 */
	int getBuffSize();
	void setBuffSize(int size);
	void setCapacity(int capacity);

	void setBuffPoint(char* buff);
	char* getBuffPoint();
	int cpyBuff(char* buff, int buff_size);

	void clear();
	virtual int add(const int key, const void* val, const int size) {return -1;};
	virtual int get(const int key, void* val) {return -1;}
};

typedef int (*callbackFun)(int, void*);

class mapParser : public parser {
protected:
public:
	mapParser();
	mapParser(char* buff, int capacity);

	int add(const int key, const void* val, const int size);
	int get(const int key, void* val);
	int getKeyMap(map<int, char*>* m);
};

class multiMapParser : public parser {
protected:
public:
	multiMapParser() {};
	multiMapParser(char* buff, int capacity);

	int add(const int key, const char* val, const int valSize);
	int get(int key, vector<char*>* val);

	void setCallback(callbackFun callback){};
};

class timeParser : public mapParser {
private:
	valTitle* latestLastTitle;
public:
	timeParser();
	timeParser(char* buff, int capacity);

	void setBuffPoint(char* buff);

	int add(int key, const char* val, const int valSize);
	int getTimeMap(map<int, char*>* m);
	int getFromTime(int time, multiMapParser* mparser);
};

int markMem(char* buff, int size);

#endif /* PARSER_H_ */
