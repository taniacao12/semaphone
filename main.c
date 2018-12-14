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
  int shmid;
  int semd;
  char * data;
  FILE * t = open("textfile", O_CREAT | 0644);

  if ((semd = semget(KEY, 1, 0)) == -1) {
    printf("Error %d in main: %s\n", errno, strerror(errno));
    exit(1);
  }
  printf("Got here 23\n");
  if ((shmid = shmget(KEY, sizeof(KEY), 0644)) == -1) {
    printf("Error %d in main: %s\n", errno, strerror(errno));
    exit(1);
  }
  printf("Got here 28\n");

  struct sembuf sb;
  sb.sem_num = 0;
  sb.sem_flg = SEM_UNDO;
  sb.sem_op = -1;
  semop(semd, &sb, 1);
  printf("Got here 35\n");
  printf("Would you like to read or write?\n(W)rite\n(R)ead\n");
  char choice[10];
  fgets(choice, 10, stdin);
  choice[strcspn(choice, "\n")] = '\0';
  printf("Got here 40\n");
  if (!strcmp(choice, "W")) {
    FILE * f = open("textfile", O_RDWR, 0644);
    printf("Writing to file\n");
    if ((data = shmat(shmid, (void *)0, 0)) == -1) {
      printf("Error %d in main: %s\n", errno, strerror(errno));
      exit(1);
    }
    printf("Got here 47\n");
    printf("Currently, this is in file\n%s\n", data);
    printf("What would you like to add?\n");
    char add[256];
    fgets(add, 256, stdin);
    add[strcspn(add, "\n")] = '\0';

    strcpy(data, add);
    int wr;
    /*if ((wr = fputs(add, f)) == -1) {              //gives seg fault, do not know why
      printf("Error %d in main: %s\n", errno, strerror(errno));
      exit(1);
    }*/
  }else{
    printf("Reading from file\n");
    FILE * f = open("textfile", O_RDONLY, 0644);
    if (f == -1) {
      printf("Error %d in main: %s\n", errno, strerror(errno));
      exit(1);
    }
    printf("Got here 62\n");
    char reading[1024];
    int re = read(f, reading, 1023);
    if (re == -1) {
      printf("Error %d: %s\n", errno, strerror(errno));
      exit(1);
    }
    printf("Got here 69\n");
    if (re != NULL) {
      printf("This was read:\n%s\n", re);
    }else{
      printf("Nothing read. Add something to begin.\n");
    }
  }

  sb.sem_op = 1;
  semop(semd, &sb, 1);

  return 0;
}
