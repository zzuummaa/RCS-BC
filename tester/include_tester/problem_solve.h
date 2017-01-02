/*
 * problem_solve.h
 *
 *  Created on: 30 ���. 2016 �.
 *      Author: Cntgfy
 */

#ifndef INCLUDE_TESTER_PROBLEM_SOLVE_H_
#define INCLUDE_TESTER_PROBLEM_SOLVE_H_

#define MODE_NORMAL				0
#define MODE_NOT_RESPOND		1
#define MODE_INCORRECT			2
#define MODE_ERROR				3
#define MODE_NOT_INITTED		4

#define SOL_NOTHING				0
#define SOL_RESTART_PROCESS		1
#define SOL_REBOOT				2

typedef struct {
	//����� ������ ����������
	int subsys_mode;
} process_condition;

typedef struct {
	int pid;
	//�������, ������� ���� ���������
	int solution;
	//����� ���������� ������ � c��
	int resp_time;
	//��������� ��������� �������
	process_condition con;
} process_info;

typedef struct {
	char* path;
	char** cmd;
} program_info;

void probSolv_initProcInfo(process_info *proci);

void probSolv_updateMode(process_info *proci, int new_mode);

int probSolv_applySolution(program_info *progi, process_info *proci);

#endif /* INCLUDE_TESTER_PROBLEM_SOLVE_H_ */
