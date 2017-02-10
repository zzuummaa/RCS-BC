//============================================================================
// Name        : rediska.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <redis3m/redis3m.hpp>
#include <iostream>
#include <unistd.h>
#include <string.h>

using namespace redis3m;
using namespace std;

int main(int argc, char **argv)
{
        connection::ptr_t conn = connection::create();

        int memSize = 3;
        char str[memSize];
        memset(str, 'a', memSize - 1);
        str[memSize - 1] = '\0';

        int i = 0;
        int lastI = 0;
        time_t timer = time(0);

		while(1) {
			conn->run(command("SET") << to_string(i) << string(str));

			if (difftime(time(0), timer) >= 1) {
				printf("Write %d records, %d bytes, cur i=%d\n", i - lastI, i * sizeof(str), i);
				lastI = i;
				timer = time(0);
				//usleep(10000);
			}

			i++;
		}

        reply r = conn->run(command("GET") << "foo" );
        std::cout << "FOO is: " << r.str() << std::endl;
}

