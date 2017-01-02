/*
 * problem_solve.cpp
 *
 *  Created on: 31 дек. 2016 г.
 *      Author: Cntgfy
 */

#include <stdio.h>
#include <cstdlib>
#include <sys/wait.h>
#include <unistd.h>

#include "problem_solve.h"

void restart_process(program_info *progi, process_info *proci) {
	if (kill(proci->pid, SIGQUIT) == -1) {
		printf("Error killing process with PID=%d\n", proci->pid);
		return;
	}

	int pid = fork();
	if (pid == 0) {
		printf("Process restarted\n");
		fflush(stdout);
		setsid();

		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		execv(progi->path, progi->cmd);
		exit(0);
	} else {
		proci->pid = pid;
	}

}

/**
 * return applied solution
 */
int probSolv_applySolution(program_info *progi, process_info *proci) {
	switch (proci->con.subsys_mode) {

	case MODE_NORMAL:
		if (proci->solution == SOL_RESTART_PROCESS) {
			proci->solution = SOL_NOTHING;
			return SOL_NOTHING;
		}
		if (proci->solution == SOL_NOTHING) {
			return SOL_NOTHING;
		}
		return SOL_NOTHING;

	case MODE_ERROR:
		if (proci->solution == SOL_NOTHING) {
			proci->solution = SOL_RESTART_PROCESS;
			restart_process(progi, proci);
			return SOL_RESTART_PROCESS;
		}
		return SOL_RESTART_PROCESS;

	case MODE_NOT_RESPOND:
		if (proci->solution == SOL_NOTHING) {
			proci->solution = SOL_RESTART_PROCESS;
			restart_process(progi, proci);
			return SOL_RESTART_PROCESS;
		}
		return SOL_RESTART_PROCESS;
	}

	return SOL_NOTHING;
}

/**
 * Install the new mode value by updating the rules
 *
 * new_mode - last received mode value, that need to be set to process_info
 */
void probSolv_updateMode(process_info *proci, int new_mode) {
	proci->con.subsys_mode = new_mode;
}

void probSolv_initProcInfo(process_info *proci) {
	proci->con.subsys_mode = MODE_NOT_INITTED;
	proci->solution = SOL_NOTHING;
}
