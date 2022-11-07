// #include <stdint.h>
// #include <stdio.h>
// #include <tests.h>
// #include <processes.h>


// #define MINOR_WAIT 1000000                               // TODO: To prevent a process from flooding the screen
// #define WAIT      10000000                              // TODO: Long enough to see theese processes beeing run at least twice

// int prioGetpid() {
//     return getPid();
// }

// int prioNice(int pid, int newPrio) {
//     changeProcessPrioritySyscall(pid, newPrio);
//     return 0;
// }

// int prioKill(int pid) {
//     killProcessSyscall(pid);
//     return 0;
// }

// int prioBlock(int pid) {
//     changeProcessStatusSyscall(pid);
//     return 0;
// }

// // int prioUnblock(int pid) {
// //     changeProcessState(pid);
// //     return 0;
// // }

// void prioBusyWait(int n) {
//     int i;
//     for (i = 0; i < n; i++);
// }

// void prioEndlessLoop() {
//     int pid = prioGetpid();

//     while (1) {
//         char b[10]={0};
//         itoaTruncate(pid, b, 10);
//         print(b);
//         prioBusyWait(MINOR_WAIT);
//     }
// }

// #define TOTAL_PROCESSES 3

// void testPrio() {
//     uint64_t pids[TOTAL_PROCESSES];
//     uint64_t i;

//     // for (i = 0; i < TOTAL_PROCESSES; i++)
//     //     pids[i] = prioCreateProcess((uint64_t * ) & prioEndlessLoop);

//     prioBusyWait(WAIT);
//     println("");
//     println("CHANGING PRIORITIES...");

//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         switch (i % 3) {
//             case 0:
//                 prioNice(pids[i], 1); //lowest priority
//                 break;
//             case 1:
//                 prioNice(pids[i], 3); //medium priority
//                 break;
//             case 2:
//                 prioNice(pids[i], 5); //highest priority
//                 break;
//         }
//     }

//     prioBusyWait(WAIT);
//     println("");
//     println("BLOCKING...");

//     for (i = 0; i < TOTAL_PROCESSES; i++)
//         prioBlock(pids[i]);

//     println("CHANGING PRIORITIES WHILE BLOCKED...");
//     for (i = 0; i < TOTAL_PROCESSES; i++) {
//         switch (i % 3) {
//             case 0:
//                 prioNice(pids[i], 1); //medium priority
//                 break;
//             case 1:
//                 prioNice(pids[i], 1); //medium priority
//                 break;
//             case 2:
//                 prioNice(pids[i], 1); //medium priority
//                 break;
//         }
//     }

//     println("UNBLOCKING...");

//     for (i = 0; i < TOTAL_PROCESSES; i++)
//         prioUnblock(pids[i]);

//     prioBusyWait(WAIT);
//     println("");
//     println("KILLING...");

//     for (i = 0; i < TOTAL_PROCESSES; i++)
//         prioKill(pids[i]);
// }

// // #define TOTAL_PROCESSES 3

// // void test_prio(){
// //   int pids[TOTAL_PROCESSES];
// //   int i;

// //   for(i = 0; i < TOTAL_PROCESSES; i++)

// //   bussy_wait(WAIT);
// //   printf("\nCHANGING PRIORITIES...\n");

// //   for(i = 0; i < TOTAL_PROCESSES; i++){
// //     switch (i % 3){
// //       case 0:
// //         my_nice(pids[i], 1); //lowest priority 
// //         break;
// //       case 1:
// //         my_nice(pids[i], 5); //medium priority
// //         break;
// //       case 2:
// //         my_nice(pids[i], 10); //highest priority
// //         break;
// //     }
// //   }

// //   bussy_wait(WAIT);
// //   printf("\nBLOCKING...\n");

// //   for(i = 0; i < TOTAL_PROCESSES; i++)
// //     my_block(pids[i]);

// //   printf("CHANGING PRIORITIES WHILE BLOCKED...\n");
// //   for(i = 0; i < TOTAL_PROCESSES; i++){
// //     switch (i % 3){
// //       case 0:
// //         my_nice(pids[i], 3); //medium priority 
// //         break;
// //       case 1:
// //         my_nice(pids[i], 3); //medium priority
// //         break;
// //       case 2:
// //         my_nice(pids[i], 3); //medium priority
// //         break;
// //     }
// //   }

// //   printf("UNBLOCKING...\n");

// //   for(i = 0; i < TOTAL_PROCESSES; i++)
// //     my_unblock(pids[i]);

// //   bussy_wait(WAIT);
// //   printf("\nKILLING...\n");

// //   for(i = 0; i < TOTAL_PROCESSES; i++)
// //     my_kill(pids[i]);

// //   printf("Passed\n");  
// //   processKiller();
// // }