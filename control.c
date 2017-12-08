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

#define SEMKEY 123
#define SHMKEY 456

//helper fxns
int set_semaphore(int id, int val){
  return semctl(id, 0, SETVAL, val);
}

int create(){
	//1. CREATING SHARED MEMORY
	int shmd_id;
	shmd_id = shmget(SHMKEY, sizeof(double), IPC_CREAT | 0600);
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
  }

	//3. OPENING FILE

	return shmd_id;
}

int main(int numargs, char ** args) {
	if (numargs < 2){
		printf("You need more arguments!\n");
	}
	else {
		if (strcmp(args[1], "-c") == 0){
			printf("%d\n", create());
		}
		// if (strcmp(args[1], "-v") == 0){
		// 	view();
		// }
		// if (strcmp(args[1], "-r") == 0){
		// 	remove();
		// }
	}
	return 0;
}
