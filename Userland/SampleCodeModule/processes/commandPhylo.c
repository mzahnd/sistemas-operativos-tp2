// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 *
 * Dijkstra's solution
 * See: https://en.wikipedia.org/wiki/Dining_philosophers_problem#Dijkstra's_solution
 */
#include <stdio.h>
#include <stdlib.h>
#include <processManagement.h>
#include <semaphoreUser.h>
#include <stringUtils.h>
#include <stdbool.h>
#include <syscalls_asm.h>

/* ------------------------------ */

#define MAX_PHYLOS 15
#define MIN_PHYLOS 3
#define INITIAL_PHYLOS 10

#define PHYLO_EAT_AT_LEAST_FOR 1
#define PHYLO_EAT_AT_MOST_FOR 2
#define PHYLO_THINK_AT_LEAST_FOR 4
#define PHYLO_THINK_AT_MOST_FOR 2

#define TABLE_MAX_PHYLOS_PER_ROWS 5
#define TABLE_PRINT_SLEEP 1

#define SEM_PHYLO_NAME "_phylo_sem"
#define SEM_TABLE_NAME "_phylo_sem_table"
#define SEM_PRINT_NAME "_phylo_sem_print"

#define PHYLOS_NAMES_NAME 0
#define PHYLOS_NAMES_LEAVE 1

#define LEFT(i) (((i) + n_philosophers_table - 1) % n_philosophers_table)
#define RIGHT(i) (((i) + 1) % n_philosophers_table)

#define ARRSIZE(a) (sizeof(a) / sizeof(*a))

/* ------------------------------ */

static const char *phylos_names[][2] = {
        { "Seneca", "was forced to commit suicide" },
        { "Plato", "is about to leave" },
        { "Hegel", "has died of cholera" },
        { "Kant", "has eat too much and is about to leave" },
        { "Nietzsche",
          "suffered a mental breakdown and needs to get some rest" },
        { "Russell", "had a new idea and is rushing home to write about it" },
        { "Ortega y Gasset",
          "has to spend some time in exile and will leave soon" },
        { "Rousseau",
          "is being denounced as the Antichrist: he has to run away!" },
        { "Voltaire", "'s affair with Pimpette has been discovered."
                      " He should explain some things..." },
        { "Socrates", "wants to proclaim his total ignorance (again)" },
        { "Descartes", "is very cold and needs some rest" },
        { "Leibniz", "died. Will someone attend his funeral?" },
        { "Diogenes",
          "is barking at another dog... Oh... Now he's chasing it" },
        { "Cicero", "was beheaded by two killers (has Marcus sent them?)" },
        { "Hypatia", "was lynched by a mob of Christians" },
        { "Aristotle", "is packing his stuff" },
        { "Mysterious Philosopher", "is hidding in the shadows" }
};

/* ------------------------------ */

enum state { DEAD = 0, THINKING, HUNGRY, EATING, KILLED };

typedef struct PHYLO {
        int state;
        sem_t both_forks_available;
} phylo_t;

/* ------------------------------ */

static phylo_t philosophers_table[MAX_PHYLOS] = {};
static unsigned int n_philosophers_table = 0;
static unsigned int n_initial_philosophers_table = 0;

static unsigned int phylos_pid[1000] = { 0 };
static unsigned int total_invoked_phylos = 0;
static unsigned int table_pid = 0;

static bool phylo_alive = 0; // To exit processes on 'q'

/* ------------------------------ */

int print_table(int argc, char **argv)
{
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return 1;
        }

        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 0);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return 1;
        }

        while (phylo_alive == true) {
                sem_wait(sem_philosophers_table);
                sem_wait(sem_print);

                printf("\n---------------\n");

                for (int i = 0, phylos_printed = 0; i < MAX_PHYLOS; i++) {
                        if (philosophers_table[i].state == DEAD)
                                continue;
                        else if (philosophers_table[i].state == EATING)
                                printf("E ");
                        else
                                printf(". ");

                        phylos_printed++;

                        if (phylos_printed == TABLE_MAX_PHYLOS_PER_ROWS) {
                                phylos_printed = 0;
                                putChar('\n');
                        }
                }

                sem_post(sem_print);
                sem_post(sem_philosophers_table);

                sleep(TABLE_PRINT_SLEEP);
        }

        return 0;
}

/* ------------------------------ */

static void print_table_in_background()
{
        char *argv_print_table[] = { "print_table", 0 };
        table_pid = createProcess(argv_print_table[0], print_table, 0,
                                  argv_print_table, 0);
}

/* ------------------------------ */

static void try_forks(unsigned index)
{
        if (philosophers_table[index].state == HUNGRY &&
            philosophers_table[LEFT(index)].state != EATING &&
            philosophers_table[RIGHT(index)].state != EATING) {
                philosophers_table[index].state = EATING;

                sem_post(&philosophers_table[index].both_forks_available);
        }
}

/* ------------------------------ */

static const char **get_philosopher_name(const int index)
{
        int name_index = index;
        if (index > ARRSIZE(phylos_names)) {
                name_index = ARRSIZE(phylos_names) - 1;
        }

        return phylos_names[name_index];
}

/* ------------------------------ */

int philosopher(int argc, char **argv)
{
        const int index = atoi(argv[1]);
        phylo_t *phil = &philosophers_table[index];

        sem_t *sem_forks = sem_open(SEM_PHYLO_NAME, 0);
        if (sem_forks == NULL) {
                printf("Failed: sem_forks\n");
                return 1;
        }

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return 1;
        }

        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 0);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return 1;
        }

        phil->state = THINKING;
        sem_init_bin(&phil->both_forks_available, 0);

        if (n_initial_philosophers_table >= INITIAL_PHYLOS) {
                sem_wait(sem_print);

                printf("%s has arrived!\n" // -V576
                       "There are %d philosophers.\n",
                       get_philosopher_name(index)[PHYLOS_NAMES_NAME],
                       n_philosophers_table);

                sem_post(sem_print);
        } else if (n_initial_philosophers_table == INITIAL_PHYLOS - 1) {
                // Philosophers joined for the first time
                sem_wait(sem_print);

                printf("The first %d philosophers " // -V576
                       "are seated the table: \n",
                       n_philosophers_table);
                for (int i = 0; i < MAX_PHYLOS; i++) {
                        if (philosophers_table[i].state != DEAD &&
                            philosophers_table[i].state != KILLED) {
                                printf("\t %s\n",
                                       get_philosopher_name(
                                               i)[PHYLOS_NAMES_NAME]);
                        }
                }

                sem_post(sem_print);

                n_initial_philosophers_table++;
        } else {
                // Philosophers joining for the first time
                n_initial_philosophers_table++;
        }

        while (phil->state != KILLED) {
                int think_time = PHYLO_THINK_AT_LEAST_FOR +
                                 rand() % PHYLO_THINK_AT_MOST_FOR;
                sleep(think_time);

                // Try to take forks
                sem_wait(sem_forks);
                if (phil->state == KILLED) { // -V547
                        // V547: This expression CAN be true because the variable is controlled
                        // by another process
                        sem_post(sem_forks);
                        break;
                }

                phil->state = HUNGRY;
                try_forks(index);
                sem_post(sem_forks);

                // Block if it could not acquire forks
                sem_wait(&phil->both_forks_available);
                if (phil->state == KILLED) // -V547
                        // V547: This expression CAN be true because the
                        // variable is controlled by another process
                        break;

                // Eat
                int eat_time =
                        PHYLO_EAT_AT_LEAST_FOR + rand() % PHYLO_EAT_AT_MOST_FOR;
                sleep(eat_time);

                // remove_philosopher() could have been called while sleeping
                if (phil->state == KILLED) // -V547
                        // V547: This expression CAN be true because the
                        // variable is controlled by another process
                        break;

                // Put down forks
                sem_wait(sem_forks);
                if (phil->state == KILLED) { // -V547
                        // V547: This expression CAN be true because the variable is controlled
                        // by another process
                        sem_post(sem_forks);
                        break;
                }

                phil->state = THINKING;

                // Verify if neighbours can eat
                try_forks(LEFT(index));
                try_forks(RIGHT(index));

                sem_post(sem_forks);
        }

        // Free resources and exit
        sem_destroy(&phil->both_forks_available);

        sem_wait(sem_philosophers_table);
        n_philosophers_table -= 1;

        sem_wait(sem_print);
        printf("%s has left." // -V576
               " %d philosophers remain.\n", // -V576
               get_philosopher_name(index)[PHYLOS_NAMES_NAME],
               n_philosophers_table);
        sem_post(sem_print);

        sem_post(sem_philosophers_table);

        phil->state = DEAD;

        return 0;
}

/* ------------------------------ */

static void remove_philosopher()
{
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return;
        }

        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 0);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return;
        }

        sem_wait(sem_philosophers_table);

        // Count alive philosophers
        int alive = 0;
        for (int i = 0; i < MAX_PHYLOS; i++) {
                if (philosophers_table[i].state != DEAD &&
                    philosophers_table[i].state != KILLED)
                        alive++;
        }

        if (n_philosophers_table <= MIN_PHYLOS) {
                sem_wait(sem_print);
                printf("It would be rude to leave your friends " // -V576
                       "eating alone.\n"
                       "(There are only %d philosophers)\n",
                       n_philosophers_table);
                sem_post(sem_print);

                sem_post(sem_philosophers_table);
                return;
        } else if (alive <= MIN_PHYLOS) {
                sem_wait(sem_print);
                printf("It would be rude to leave your friends "
                       "eating alone after those leaving have retired.\n");
                sem_post(sem_print);

                sem_post(sem_philosophers_table);
                return;
        }

        int remove = 0;
        do {
                remove = rand() % MAX_PHYLOS;
        } while (philosophers_table[remove].state == DEAD ||
                 philosophers_table[remove].state == KILLED ||
                 philosophers_table[remove].state == EATING);

        philosophers_table[remove].state = KILLED;
        // Release it if needed
        sem_post(&philosophers_table[remove].both_forks_available);

        sem_wait(sem_print);
        printf("%s %s\n", get_philosopher_name(remove)[PHYLOS_NAMES_NAME],
               get_philosopher_name(remove)[PHYLOS_NAMES_LEAVE]);
        sem_post(sem_print);

        sem_post(sem_philosophers_table);
}

/* ------------------------------ */

static void add_philosopher()
{
        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 0);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return;
        }

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return;
        }

        int argc = 3;
        char **argv = calloc(3, sizeof(char *));
        if (argv == NULL)
                return;

        sem_wait(sem_philosophers_table);

        if (n_philosophers_table == MAX_PHYLOS) {
                sem_wait(sem_print);
                printf("Agh! There is no more room!\n");
                sem_post(sem_print);

                sem_post(sem_philosophers_table);

                free(argv);
                return;
        }

        argv[0] = calloc(strlen("philosopher") + 1, sizeof(char));
        if (argv[0] == NULL) {
                free(argv);
                return;
        }

        memcpy(argv[0], "philosopher", strlen("philosopher") + 1);

        argv[1] = calloc(16, sizeof(char));
        if (argv[1] == NULL) {
                free(argv[0]);
                free(argv);
                return;
        }

        int add_index = 0;
        if (n_initial_philosophers_table < INITIAL_PHYLOS) {
                // Initial case
                add_index = n_philosophers_table;
        } else {
                int counted = 0;
                while (philosophers_table[add_index].state != DEAD &&
                       counted < MAX_PHYLOS) {
                        add_index++;
                        counted++;
                }

                if (counted == MAX_PHYLOS) {
                        sem_wait(sem_print);
                        printf("Mmmh... It seems that all chairs have a body in it.\n"
                               "Wait until a philosopher leaves"
                               " (or a dead body is removed...)\n");
                        sem_post(sem_print);

                        free(argv[0]);
                        free(argv[1]);
                        free(argv);

                        sem_post(sem_philosophers_table);
                        return;
                }
        }

        intToString(add_index, argv[1]);

        argv[2] = 0;

        n_philosophers_table += 1;
        phylos_pid[total_invoked_phylos] =
                createProcess(argv[0], &philosopher, argc, argv, 0);
        total_invoked_phylos++;
        sem_post(sem_philosophers_table);
}

/* ------------------------------ */

static bool readKeyboard()
{
        int c = getChar();

        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 0);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return false;
        }

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return false;
        }

        switch (c) {
        case 'a':
                add_philosopher();
                break;

        case 'r':
                remove_philosopher();
                break;

        case 'q':
                sem_wait(sem_philosophers_table);
                sem_wait(sem_print);

                printf("Everybody is full, they're all leaving!\n");
                return false;

        default:
                break;
        }

        return true;
}

/* ------------------------------ */

void print_welcome()
{
        printf("\n");
        printf("Yeah!\n");
        printf("Hello!\n");
        printf("Here I am, here we are, we are one\n");
        printf("I've been waiting for this night to come\n");
        printf("Get up!\n");
        printf("\n");
        printf("You're in the psycho circus\n");
        printf("And I say welcome to the show\n");
        printf("\n");

        printf("Use the 'a' key to add philosophers to the table\n");
        printf("Use the 'r' key to remove philosophers from the table\n");
        printf("Enjoy your meal!\n");
        printf("\n");
}

/* ------------------------------ */

int commandPhylo(int argc, char **argv)
{
        // ""Random seed"". This gives us some predictability
        srand(38403);
        n_philosophers_table = 0;

        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 1);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return 1;
        }

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return 1;
        }

        sem_t *sem_forks = sem_open(SEM_PHYLO_NAME, 0);
        if (sem_forks == NULL) {
                printf("Failed: sem_forks\n");
                return 1;
        }

        print_welcome();

        for (int i = 0; i < INITIAL_PHYLOS; i++) {
                add_philosopher();
        }
        sem_post(sem_forks);

        phylo_alive = true;

        print_table_in_background();

        // Listen for keyboard events
        int kb = true;
        while (kb == true) {
                kb = readKeyboard();
        }

        // Finish process
        phylo_alive = false;

        for (int i = 0; i < n_philosophers_table; i++) {
                philosophers_table[i].state = DEAD;
                sem_destroy(&philosophers_table[i].both_forks_available);
        }

        n_philosophers_table = 0;
        n_initial_philosophers_table = 0;

        for (int i = 0; i < total_invoked_phylos; i++) {
                killProcessSyscall(phylos_pid[i]);
        }
        killProcessSyscall(table_pid);

        sem_close(sem_forks);
        sem_close(sem_philosophers_table);
        sem_close(sem_print);

        printf("Good bye!\n");
        return 0;
}
