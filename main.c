#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int main() {
  int shmid = shmget(KEY, 100, 0);
  int semid = semget(KEY, 1, 0);
  int file = open("test.txt", O_WRONLY|O_APPEND);
  if(shmid == -1 || semid == -1 || file == -1){
    printf("Error: %s",strerror(errno));
  }
  else{
    struct sembuf semb;
    semb.sem_num = 0;
    semb.sem_flg = SEM_UNDO;
    semb.sem_op = -1;
    semop(semid, &semb, 1);
    char * data = shmat(shmid, 0, 0);
    printf("This was the last line": \n%s\n",data);
    printf("Next line:\n");
    char buffer[1000];
    fgets(buffer, 1000, stdin);
    strcpy(data, buffer);
    write(file, buffer, strlen(buffer));
    close(file);
    semb.sem_op = 1;
    semop(semid, &semb, 1);
    return 0;
  }
}
