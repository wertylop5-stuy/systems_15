#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

#include"include/consts.h"

#define SEMKEY 123
#define SHMKEY 456
#define FILENAME ""
#define LINE_BUF_SIZE 512

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

int main() {
	int semd = semget(SEMKEY, 1, 0600);
	
	struct sembuf buf = {0, -1, SEM_UNDO};
	
	//supposed to be a struct sembuf array? But we only use one,
	//so we won't use the array
	printf("[%d] getting semaphore\n", semd);
	if (semop(semd, &buf, 1) == -1) {
		printf("%d: %s\n", errno, strerror(errno));
		exit(1);
	}
	printf("[%d] got semaphore\n", semd);
	
	char line_buf[LINE_BUF_SIZE];
	int shmd = shmget(SHMKEY, sizeof(line_buf), 0600);
	
	
	return 0;
}

