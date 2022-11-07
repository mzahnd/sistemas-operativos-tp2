// #include <stdio.h>
// #include <test_util.h>
// #include <processes.h>
// #include <syscalls_asm.h>
// #include <tests.h>

// static int my_kill(int pid);
// static int my_block(int pid);
// static int my_unblock(int pid);


// //TO BE INCLUDED
// static void endless_loop(){
//   while(1);
// }

// static int my_kill(int pid){
//   return kill(pid);
// }

// static int my_block(int pid){
//   return block(pid);
// }

// static int my_unblock(int pid){
//   return unblock(pid);
// }

// #define MAX_PROCESSES 10 //Should be around 80% of the the processes handled by the kernel

// enum State {ERROR, RUNNING, BLOCKED, KILLED};

// typedef struct P_rq{
//   int pid;
//   enum State state;
// }p_rq;

// void test_processes(){
//   p_rq p_rqs[MAX_PROCESSES];
//   int rq;
//   int alive = 0;
//   int action;

//     // Randomly kills, blocks or unblocks processes until every one has been killed
//     while (alive > 0){

//       for(rq = 0; rq < MAX_PROCESSES; rq++){
//         action = GetUniform(2) % 2; 

//         switch(action){
//           case 0:
//             if (p_rqs[rq].state == RUNNING || p_rqs[rq].state == BLOCKED){
//               if (my_kill(p_rqs[rq].pid) == -1){          // TODO: Port this as required
//                 printf("Error killing process\n");        // TODO: Port this as required
//                 return;
//               }
//               p_rqs[rq].state = KILLED; 
//               alive--;
//             }
//             break;

//           case 1:
//             if (p_rqs[rq].state == RUNNING){
//               if(my_block(p_rqs[rq].pid) == -1){          // TODO: Port this as required
//                 printf("Error blocking process\n");       // TODO: Port this as required
//                 return;
//               }
//               p_rqs[rq].state = BLOCKED; 
//             }
//             break;
//         }
//       }

//       // Randomly unblocks processes
//       for(rq = 0; rq < MAX_PROCESSES; rq++)
//         if (p_rqs[rq].state == BLOCKED && GetUniform(2) % 2){
//           if(my_unblock(p_rqs[rq].pid) == -1){            // TODO: Port this as required
//             printf("Error unblocking process\n");         // TODO: Port this as required
//             return;
//           }
//           p_rqs[rq].state = RUNNING; 
//         }
//     } 
//     printf("Passed\n");
//     processKiller();
// }