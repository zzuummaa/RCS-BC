/*
 * tester_main.cpp
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	if (fork() == 0) {
		printf("Goodby, I'm parrent\n");
		exit(0);
	}

	setsid();

	char* cmd[] = {"telemetry_demon"};
	execv("/home/pi/rcs/telemetry", cmd);

	return 0;
}
