#include<stdio.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/stat.h>

#include"include/consts.h"

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short *array;
	struct seminfo *__buf;
};

void err_exit() {
	printf("%d: %s\n", errno, strerror(errno));
	exit(1);
}

int main() {
	int semd = semget(SEMKEY, 1, 0600);
	struct sembuf buf = {0, -1, SEM_UNDO};
	
	//supposed to be a struct sembuf array? But we only use one,
	//so we won't use the array
	printf("[%d] getting semaphore\n", semd);
	if (semop(semd, &buf, 1) == -1) err_exit();
	printf("[%d] got semaphore\n", semd);
	
	//read shared mem
	char *line_buf;
	int shmd = shmget(SHMKEY, LINE_BUF_SIZE, 0600);
	printf("[%d] reading shared mem\n", shmd);
	if (shmd == -1) err_exit();
	line_buf = (char*)shmat(shmd, 0, 0);
	
	//get input
	printf("last line:\n%s\n", line_buf);
	printf("Type in the next part of the story:\n");
	fgets(line_buf, LINE_BUF_SIZE, stdin);
	
	//write to file
	int fd = open(FILE_NAME, O_WRONLY | O_APPEND);
	if (write(fd, line_buf, strlen(line_buf)) == -1) err_exit();
	close(fd);
	
	//detach shared mem
	printf("[%d] detach shared mem\n", shmd);
	shmdt(line_buf);
	line_buf = 0;
	
	//release semphore
	buf.sem_op = 1;
	printf("[%d] releasing semaphore\n", semd);
	if (semop(semd, &buf, 1) == -1) err_exit();
	
	return 0;
}

