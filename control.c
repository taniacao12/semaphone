#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#define KEY 1337

union semun {
	int val;
	struct semid_ds *buf;
	unsigned short  *array;
	struct seminfo  *__buf;
};

int main(){
	char * commands = malloc(256);
	printf("--------------------------------------------------\n");
	printf("COMMANDS\n create : -C\n remove : -r\n view :   -v\n");
	printf("--------------------------------------------------\n");
	printf("What is your command? > ");
	scanf("%s", commands);
	while(1){
		int shared_mem = shmget(KEY,1024,IPC_CREAT | 0644);
		char *str = (char*)shmat(shared_mem,0,0);
		int file = open("file.txt", O_CREAT | O_APPEND | 0644);
		int semaphore = semget(KEY,2,IPC_CREAT | 0644);
		if(!strcmp("-C", commands)){
			str = malloc(256);
			printf("write something:\n");
			scanf("%s",str);
			free(str);
			shmdt(shared_mem);
			printf("write to file:\n");
			scanf("%s",str);
			write(file, str, sizeof(*str));
			close(file);
			break;
		}
		else if(!strcmp("-v", commands)){
			char *str;
			int seg_id;
			//printf("What is the segment's ID? > ");
			//scanf("%d",seg_id);
			int shared_mem = shmget(KEY,1024,0);
			str = (char*)shmat(shared_mem,0,0);
			printf("%s\n",str);
			shmdt(shared_mem);
			break;
		}
		else if (!strcmp("-r", commands)) {
			int shared_mem = shmget(KEY,1024,0);
			str = (char*)shmat(shared_mem,0,0);
			printf("This is the story you are about to remove: %s\n", str);
        shmdt(str);
        shmctl(shared_mem, IPC_RMID, 0);
        semctl(semaphore, 0, IPC_RMID, 0);
        fclose(file);
        remove("file.txt");
      }
	else{
		printf("What is your command? > ");
		free(commands);
	}
	scanf("%s",commands);
}
	return 0;
}
