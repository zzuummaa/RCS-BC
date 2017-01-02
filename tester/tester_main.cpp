/*
 * tester_main.cpp
 *
 *  Created on: 30 дек. 2016 г.
 *      Author: Cntgfy
 */

#include "stdio.h"
#include <signal.h>
#include <stdlib.h>

#include "include_tester/problem_solve.h"
#include "include_tester/camera_manager.h"
#include "pipe.h"

void updateMode(pipe_pack *pp, process_info *proci) {
	switch (pp->type) {
	case TYPE_CAMERA : cameraManager_updateProcInfo( (camera_info*)pp->data, proci ); break;
	}

}

void remove_pipe(int status) {
	printf("Signal quit\n");
	pipe_remove(PIPE_TESTER);
	exit(0);
}

int main() {
	signal(SIGTERM, remove_pipe);

	pipe_make(PIPE_TESTER);
	printf("Pipe waiting for writers...\n");
	int fd = pipe_openReadOnly(PIPE_TESTER);
	if (fd != -1) {
		printf("Pipe opened\n");
	} else {
		printf("Pipe error opening\n");
		return 1;
	}

	program_info camprogi;
	cameraManager_initProgInfo(&camprogi);

	process_info camproci;
	probSolv_initProcInfo(&camproci);

	while (1) {
		pipe_pack pp;
		if (pipe_read(fd, &pp) == 0) continue;
		printf("Received pack with type=%d\n", pp.type);

		updateMode(&pp, &camproci);
		probSolv_applySolution(&camprogi, &camproci);
	}

	return 0;
}
