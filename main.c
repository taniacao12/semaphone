#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>

#define STR_LEN 200

int main() {
    key_t key;
    key = ftok("makefile", 'J');

    int shmid;
    char *data;
    char input[STR_LEN];
    char new_seg = 1;

    while (1) {
        shmid= shmget(key, STR_LEN, 0644 | IPC_CREAT);
        data = shmat(shmid, (void *) 0, 0);

        if (new_seg)
					printf("----- A NEW SEGMENT HAS BEEN CREATED -----\n");
        else
					printf("----- EXITISTING SEGMENT -----\n");

        new_seg = 0;
        input[0] = 0;

        while (strcmp(input, "y") && strcmp(input, "n")) {
            printf("Do you want to change the data in the segment? (y/n)\n");
            fgets(input, STR_LEN, stdin);
            input[strlen(input) - 1] = 0;
        }

        if (!strcmp(input, "y")) {
            printf("What do you want to write to the segment?\n");
            fgets(data, STR_LEN, stdin);
            printf("You wrote: %s", data);
        }

        input[0] = 0;

        while (strcmp(input, "y") && strcmp(input, "n")) {
            printf("Do you want to delete this segment? (y/n)\n");
            fgets(input, STR_LEN, stdin);
            input[strlen(input) - 1] = 0;
        }

        if (!strcmp(input, "y")) {
            shmdt(data);
            shmctl(shmid, IPC_RMID, NULL);
            new_seg = 1;
            printf("segment has been deleted\n");
        }
    }

    return 0;
}