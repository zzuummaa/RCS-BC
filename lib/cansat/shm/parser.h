/*
 * parser.h
 *
 *  Created on: 30.01.2017
 *      Author: zzuummaa
 */

#ifndef PARSER_H_
#define PARSER_H_

typedef struct {
	int size;
} mapTitle;

class parser {
protected:
	char* buff;
	int capacity;
public:
	/**
	 * return size of data, contained in buff
	 *     or -1 if error occurred
	 */
	int getBuffSize() {return -1;};

	void setBuffPoint(char* buff);
	char* getBuffPoint();
	int cpyBuff(char* buff, int buff_size);
};

class mapParser : public parser {
private:
	mapTitle* mTitle;
public:
	mapParser(char* buff, int capacity);

	int add(int key, char* val, int size);
	int getBuffSize();
	void setBuffSize(int size);
	int get(int key, char** buff);
};

int markMem(char* buff, int size);

#endif /* PARSER_H_ */
