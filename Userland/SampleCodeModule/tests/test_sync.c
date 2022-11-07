// #include <stdint.h>
// #include <stdio.h>
// #include <processes.h>
// #include <stdlib.h>
// #include <stddef.h>
// #include <semaphoreUser.h>

// #define TOTAL_PAIR_PROCESSES 2
// #define SEM_ID 101

// int global;  //shared memory

// void slowInc(int *p, int inc){
//   int aux = *p;
//   aux += inc;
//   yield();
//   *p = aux;
// }

// void inc(int argc, char *argv[]){
//   int i;
//   int semId;
//   int sem = atoi(argv[1]);
//   int value = atoi(argv[2]);
//   int n = atoi(argv[3]);

//   if (sem && (semId = sem_open(SEM_ID, 1)) == -1){
//     printf("ERROR OPENING SEM\n");
//     return;
//   }
  
//   for (i = 0; i < n; i++){
//     if (sem) sem_wait(semId);
//     slowInc(&global, value);
//     if (sem) sem_post(semId);
//   }
//   if (sem) sem_close(semId);
  
//   printf("Final value: %d\n", global);
//   processKiller();
// }

// void test_sync() {
//   int i;

//   global = 0;

//   printf("CREATING PROCESSES...(WITH SEM)\n");

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     char *argv1[5] = {"inc+", "1", "1", "1000", NULL};
//     createProcess(inc, 0, argv1, NULL);
//     char *argv2[5] = {"inc-", "1", "-1", "1000", NULL};
//     createProcess(inc, 0, argv2, NULL);
//   }

//   printf("PASSED\n");

//   processKiller();
// }

// void test_no_sync() {
//   int i;

//   global = 0;

//   printf("CREATING PROCESSES...(WITHOUT SEM)\n");

//   for(i = 0; i < TOTAL_PAIR_PROCESSES; i++){
//     char *argv1[5] = {"inc+", "0", "1", "1000", NULL};
//     createProcess(inc, 0, argv1, NULL);
//     char *argv2[5] = {"inc-", "0", "-1", "1000", NULL};
//     createProcess(inc, 0, argv2, NULL);
//   }

//   printf("PASSED\n");

//   processKiller();
// }