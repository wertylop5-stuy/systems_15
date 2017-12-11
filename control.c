#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

//all of our defines are in here
#include"include/consts.h"

//helper fxns
int set_semaphore(int id, int val){
  return semctl(id, 0, SETVAL, val);
}

int create(){
	//1. CREATING SHARED MEMORY
	int shmd_id;
	shmd_id = shmget(SHMKEY, LINE_BUF_SIZE, IPC_CREAT | 0600);
	printf("getting shared mem: %d\n", shmd_id);
	int * size_of_prev;
	size_of_prev = shmat(shmd_id, NULL, 0600);

	//2. CREATING THE SEMAPHORE
  int semid;
  semid = semget(SEMKEY, 1,  IPC_CREAT | IPC_EXCL | 0664);
  //printf("semid after semget: %d\n", semid);
  //printf("error :( %s\n", strerror(errno));
  if (semid != -1){
    printf("semaphore created: %d\n", semid);
    set_semaphore(semid, 1);
    printf("value set: %d\n", 1);
  }
  else {
    printf("semaphore already exists!\n");
	return -1;
  }

	//3. OPENING FILE

	int fd = open(FILE_NAME, O_CREAT | O_TRUNC, 0644);
	if (fd == -1) {
		printf("%d: %s\n", errno, strerror(errno));
	}
	close(fd);
	
	return shmd_id;
}

void view(void) {
	int fd = open(FILE_NAME, O_RDONLY);
	if (fd == -1) {
		printf("%d: %s\n", errno, strerror(errno));
		return;
	}
	char data[1024];
	data[read(fd, data, sizeof(data))] = 0;
	close(fd);
	
	printf("story contents:\n%s", data);
}

void r(void) {
	int semd = semget(SEMKEY, 1, 0600);
	struct sembuf buf = {0, -1, SEM_UNDO};
	
	printf("[%d] getting semaphore\n", semd);
	if (semop(semd, &buf, 1) == -1) {
		printf("%d: %s\n", errno, strerror(errno));
		return;
	}
	printf("[%d] got semaphore\n", semd);

	if (semctl(semd, 0, IPC_RMID) == -1) {
		printf("%d: %s\n", errno, strerror(errno));
	}
	printf("[%d] Removed semaphore\n", semd);
	
	int shmd = shmget(SHMKEY, LINE_BUF_SIZE, 0600);
	if (shmctl(shmd, IPC_RMID, 0) == -1) {
		printf("%d: %s\n", errno, strerror(errno));
	}
	printf("[%d] Removed shared mem\n", shmd);
	
	view();
	
	//remove the story file
	remove(FILE_NAME);
}

int main(int numargs, char ** args) {
	if (numargs < 2){
		printf("You need more arguments!\n");
	}
	else {
		if (strcmp(args[1], "-c") == 0){
			printf("%d\n", create());
		}
		else if (strcmp(args[1], "-v") == 0){
			view();
		}
		if (strcmp(args[1], "-r") == 0){
		 	r();
		}
	}
	return 0;
}
