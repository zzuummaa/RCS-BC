//============================================================================
// Name        : test1.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "shm/data_service.h"

#include <iostream>
#include <chrono>
#include <unistd.h>
#include <string.h>

using namespace std;
using namespace std::chrono;

char str[] = "sas";

int initDataService(dataService** dserv, char* arg) {
	if ( !strcmp(arg, "file") ) {
		*dserv = new IPCDataService("ds_file", 100 * 1000 * 1000, MEM_FILE);
		//((IPCDataService*)*dserv)->setParser(new timeParser());
		return 1;
	}

	if ( !strcmp(arg, "shared") ) {
		*dserv = new IPCDataService(DEFAULT_D_SERV_NAME, 100 * 1000 * 1000, MEM_SHARED);
		return 1;
	}

	if ( !strcmp(arg, "gdb") ) {
		*dserv = new DBMDataService();
		return 1;
	}

	return 0;
}

int main(int argc, char* argv[]) {
	cout << "Initializing data service" << endl;

	dataService* dserv = NULL;

	if (argc < 2) {
		cout << "Need more args: <gdb|file|shared> " << endl;
		return 0;
	}

	if ( !initDataService(&dserv, argv[1]) ) {
		cout << "Invalid first arg" << endl;
		return 0;
	}

	milliseconds start_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	if ( !dserv->create() ) {
		cerr << "Error create dataService" << endl;
		return 1;
	}

	milliseconds end_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	long dserv_ms = (end_ms - start_ms).count();

	cout << "Initialize success" << endl;

	int count = 100 * 1000;

	cout << "Add " << count << " elements..." << endl;

	start_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	for (int i = 0; i < count; i++) {
		if ( i % (count / 10) == 0 ) {
			cout << (int)( (double)i / count * 100 ) << endl;
		}
		dserv->add(i, str, sizeof(str));
	}
	end_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	cout << "Add success" << endl;
	long add_ms = (end_ms - start_ms).count();

	/*cout << "Indexing added elements..." << endl;

	start_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	map<int, char*> timeMap;
	( (timeParser*)dserv->getParser() )->getTimeMap(&timeMap);
	end_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	cout << "Indexing success" << endl;
	long index_ms = (end_ms - start_ms).count();

	cout << "Reading elements" << endl;

	bool status = 1;
	map<int, char*>::iterator iter = timeMap.begin();

	start_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	while (iter != timeMap.end()) {
		if ( strcmp(str, iter->second) != 0 ) {
			status = 0;
			break;
		}
		iter++;
	}
	end_ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());

	long readelem_ms = (end_ms - start_ms).count();
	if (status == 1) {
		cout << "Reading elements success" << endl;
	} else {
		cout << "Compare error element " << iter->first << endl;
	}*/

	cout << "Initialize time: " << dserv_ms << "ms" << endl;
	cout << "Add time: " << add_ms << "ms" << endl;
	cout << "Add average speed: " << count / ( (double)add_ms ) << "(operations/ms)" << endl;
	/*cout << "Indexing time: " << index_ms << endl;
	cout << "Indexing average speed: " << count / ( (double)index_ms ) << "(operations/ms)" << endl;
	cout << "Read element time: " << readelem_ms << "ms" << endl;
	cout << "Read element average speed: " << count / ( (double)readelem_ms ) << "operations/ms" << endl;*/

	cout << "Write a symbol" << endl;

	int cc;
	cin >> cc;

	dserv->disconnect();

	return 0;
}
