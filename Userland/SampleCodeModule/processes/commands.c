// static void commandLoop(int argc, char **argv) {
//     unsigned int pid = getPid();
//     putChar('\n');
    
//     while(1) {
//         printf("Hi! You ran the loop command, the PID is: %d. To quit the loop, press F5\n", pid);

//         int ticksEnd = getTicks() + 5;
//         while(getTicks() < ticksEnd) {
//             ;
//         }
//     }
// }

// int loopFunc(int argc, char **argv) {
//     putChar('\n');
//     return buildProcess("loop", loop, args);
// }

// int commandKill(int argc, char **argv) {
//     putChar('\n');
//     unsigned int id = atoi(argv[1]);
//     if(kill(id) == 0) 
//         printf("Process successfully killed\n");
//     else
//         printf("Process kill failed\n");
    
//     return 1;
// }



// int commandBlock(int argc, char **argv) {
//     putChar('\n');
//     unsigned int id = atoi(args[1]);
//     if(block(id) == 0) {
//         printf("Process successfully blocked\n");
//     } else {
//         printf("Process failed to block\n");
//     }
    
//     return 1;
// }

// int commandUnblock(char args[MAX_ARGS][MAX_ARG_LEN]) {
//     putChar('\n');
//     unsigned int id = atoi(args[1]);
//     if(unblock(id) == 0) {
//         printf("Process successfully unblocked\n");
//     } else {
//         printf("Process failed to unblock\n");
//     }
//     return 1;
// }



// int catFunc(int argc, char **argv) {
//     return buildProcess("cat", cat, args);
// }

// static void commandWc(int argc, char **argv) {
//     putChar('\n');
//     unsigned int lines = 0;
//     char c;
//     while((c = getChar()) != (char)EOF && lines < MAX_LINES) {
//         putChar(c);
//         if(c == '\n') {
//             lines++;
//         }
//     }
//     if(lines == MAX_LINES) {
//         printf("You've reached the maximum amount of lines\n");
//     }
//     printf("\n Amount of lines: %d\n", lines);
//     processKiller();
// }

// int wcFunc(int argc, char **argv) {
//     return buildProcess("wc", wc, args);
// }

// static void commandFilter(int argc, char **argv) {
//     char c;
//     putChar('\n');

//     char buffer[50];
//     unsigned int i = 0;

//     while((c = getChar()) != '\n'){
//         putChar(c);
//         if(!isVowel(c)) {
//             // strcat(buffer, c, &i);
//             buffer[i++] = c;
//         }
//     }
//     buffer[i] = 0;
//     putChar('\n');
//     printf("Filtered word: %s\n", buffer);
//     processKiller();
// }

// int filterFunc(int argc, char **argv) {
//     return buildProcess("filter", filter, args);
// }



// int commandPhylo(int argc, char **argv) {
//     return phyloProcess("phylo", philosopherProblem(), args);
// }

// static int phyloProcess(char *name, void (*entryPoint) (/*int, */char [][MAX_ARG_LEN]), char args[][MAX_ARG_LEN]) {
//     int j = 0;
//     unsigned int argc = atoi(args[j++]);
    
//     int foreground = atoi(args[j++]);

//     int fds[2];
//     fds[0] = atoi(args[j++]);  
//     fds[1] = atoi(args[j++]);
    
//     // First arg for name
//     char argv[argc+1][MAX_ARG_LEN];

//     int inner, outer;
//     for (outer = 0; outer < MAX_ARG_LEN; outer++)
//         for (inner = 0; inner < argc+1; inner++)
//             argv[inner][outer] = 0;

//     int i = 0;
//     strcpy(argv[i], name);
//     i++;

//     for(; i < argc+1; i++) {
//         strcpy(argv[i], args[i + 3]);
//     }

//     int size = argc + 2; 
//     char *argv_p[size];
//     for(i=0; i<argc+1; i++)
//       argv_p[i] = argv[i];
//     argv_p[size - 1] = 0;
    
//     return createProcess(entryPoint, foreground, argv_p, fds);
// }


