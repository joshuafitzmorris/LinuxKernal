#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <limits.h>
#include <errno.h>
#include <sys/syscall.h>
#include "process_ancestors.h"

#define _PROCESS_ANCESTORS_TEST_ 342

int main(int argc, char *argv[])
{

	printf("PROCESS_ANCESTORS SYS-CALL TEST: \n\n");
	int result;
	long num_filled;

	const long ERROR_SIZE = -4;
	const long VALID_SIZE = 37;

	printf("ERROR_SIZE: %li\n", ERROR_SIZE);
	printf("VALID_SIZE: %li\n", VALID_SIZE);

	struct process_info info[VALID_SIZE];

	// 1. Test invalid size
	printf("Testing invalid size: \n\n");
	result = syscall(_PROCESS_ANCESTORS_TEST_, info, ERROR_SIZE, &num_filled);
	assert(result == -1);
	assert(errno == EINVAL);

	// 2. Test invalid num_filled pointer
	printf("\nTesting invalid num_filled pointer: \n\n");
	result = syscall(_PROCESS_ANCESTORS_TEST_, info, VALID_SIZE, NULL);
	assert(result == -1);
	assert(errno == EFAULT);

	// 3. Test invalid info array passed
	printf("\nTesting invalid array passed: \n\n");
	result = syscall(_PROCESS_ANCESTORS_TEST_, NULL, VALID_SIZE, &num_filled);
	assert(result == -1);
	assert(errno == EFAULT);


	// 4. Test valid size
	printf("\nTesting valid size\n\n\n");
	result = syscall(_PROCESS_ANCESTORS_TEST_, info, VALID_SIZE, &num_filled);
	assert(result == 0);


	// 5. Display process data
	printf("\nDisplay Process data: \n\n");
	for(int i=0; i<num_filled; i++) {
		printf("info[%i]: ", i);
		printf("pid: %li, name: %s, state: %li, uid: %li, nvcsw: %li, nivscw: %li, num_children: %li, num_siblings: %li\n",
			info[i].pid, info[i].name, info[i].state, info[i].uid, info[i].nvcsw, info[i].nivcsw, info[i].num_children, info[i].num_siblings);
	}


	printf("\n\nAll tests passed!\n");
	
	return 0;
}