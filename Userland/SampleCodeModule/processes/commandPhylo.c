// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/**
 * This file is part of sistemas-operativos-tp2
 * Licensed under BSD 3-Clause "New" or "Revised" License.
 * Copyright (c) 2022 Flores Levalle, M.
 *                    López, P.
 *                    Sierra Pérez, C.
 *                    Zahnd, M. E.
 */
#include <stdio.h>
#include <stdlib.h>
#include <processManagement.h>
#include <semaphoreUser.h>
#include <stringUtils.h>

/* ------------------------------ */

#define MAX_PHYLOS 16
#define MIN_PHYLOS 3
#define INITIAL_PHYLOS 5

#define TABLE_MAX_PHYLOS_PER_ROWS 5

#define SEM_PHYLO_NAME "_phylo_sem"
#define SEM_TABLE_NAME "_phylo_sem_table"
#define SEM_PRINT_NAME "_phylo_sem_print"

#define LEFT(i) (((i) + n_philosophers_table - 1) % n_philosophers_table)
#define RIGHT(i) (((i) + 1) % n_philosophers_table)

/* ------------------------------ */

enum state { DEAD = 0, THINKING, HUNGRY, EATING };

typedef struct PHYLO {
        int state;
        sem_t both_forks_available;
} phylo_t;

/* ------------------------------ */

static phylo_t philosophers_table[MAX_PHYLOS] = {};
static unsigned int n_philosophers_table = 0;

/* ------------------------------ */

static void print_table()
{
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);
        /* sem_wait(sem_print); */

        for (int i = 0; i < n_philosophers_table; i++) {
                if (philosophers_table[i].state == EATING)
                        printf("E ");
                else
                        printf(". ");
        }
        putChar('\n');

        /* sem_post(sem_print); */
}

static void try_forks(unsigned index)
{
        if (philosophers_table[index].state == HUNGRY &&
            philosophers_table[LEFT(index)].state != EATING &&
            philosophers_table[RIGHT(index)].state != EATING) {
                philosophers_table[index].state = EATING;
                sem_post(&philosophers_table[index].both_forks_available);
        }
}

static int philosopher(int argc, char **argv)
{
        const int index = atoi(argv[1]);
        phylo_t *phil = &philosophers_table[index];
        sem_t *forks_sem = sem_open(SEM_PHYLO_NAME, 1);
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);

        phil->state = THINKING;
        sem_init_bin(&phil->both_forks_available, 0);

        /* sem_wait(sem_print); */
        printf("A new philosopher has arrived!\n");
        /* sem_post(sem_print); */

        /* print_table(); */

        while (phil->state != DEAD) {
                // Thinks for 1 to 3 second;
                int think_time = (rand() + 1) % 3;
                sleep(think_time);

                // Take forks
                sem_wait(forks_sem);
                phil->state = HUNGRY;
                try_forks(index);
                sem_post(forks_sem);

                // Block if it could not acquire forks
                sem_wait(&phil->both_forks_available);

                print_table();

                // Eat
                int eat_time = (rand() + 1) % 3;
                sleep(eat_time);

                // Put down forks
                sem_wait(forks_sem);
                phil->state = THINKING;

                // Verify if neighbours can eat
                try_forks(LEFT(index));
                try_forks(RIGHT(index));

                sem_post(forks_sem);
        }

        sem_destroy(&phil->both_forks_available);

        return 0;
}

static void remove_philosopher()
{
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);
        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 1);

        sem_wait(sem_philosophers_table);
        int remove = rand() % n_philosophers_table;

        phylo_t *phil = &philosophers_table[remove];

        phil->state = DEAD;
        sem_destroy(&phil->both_forks_available);

        n_philosophers_table--;

        /* sem_wait(sem_print); */
        printf("Removed philosopher number %d\n", remove);
        printf("There are %d philosophers left\n", n_philosophers_table);
        /* sem_post(sem_print); */

        sem_post(sem_philosophers_table);
}

static void add_philosopher()
{
        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 1);

        int argc = 3;
        char **argv = calloc(3, sizeof(char *));

        sem_wait(sem_philosophers_table);
        argv[0] = calloc(strlen("philosopher") + 1, sizeof(char));
        memcpy(argv[0], "philosopher", strlen("philosopher"));

        argv[1] = calloc(16, sizeof(char));
        intToString(n_philosophers_table, argv[1]);

        argv[2] = 0;

        createProcess(argv[0], &philosopher, argc, argv, 0);

        n_philosophers_table++;
        sem_post(sem_philosophers_table);
}

static int readKeyboard()
{
        int c = getChar();
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);

        switch (c) {
        case 'a':
                if (n_philosophers_table < MAX_PHYLOS) {
                        add_philosopher();
                } else {
                        /* sem_wait(sem_print); */
                        printf("Table full.\n");
                        /* sem_post(sem_print); */
                }
                break;
        case 'r':

                if (n_philosophers_table < MIN_PHYLOS) {
                        remove_philosopher();
                } else {
                        /* sem_wait(sem_print); */
                        printf("It would be rude to live your friend "
                               "eating alone.\n");
                        /* sem_post(sem_print); */
                }
                break;
        case 'q':
                /* sem_post(sem_print); */
                printf("Good bye!");
                return 0;
        }

        return 1;
}

/* ------------------------------ */

int commandPhylo(int argc, char **argv)
{
        printf("Yeah!\n");
        printf("Hello!\n");
        printf("Here I am, here we are, we are one\n");
        printf("I've been waiting for this night to come\n");
        printf("Get up!\n");
        printf("\n");
        printf("You're in the psycho circus\n");
        printf("And I say welcome to the show\n");
        printf("\n");

        // ""Random seed""
        srand(38403);
        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 1);

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);

        sem_t *forks_sem = sem_open(SEM_PHYLO_NAME, 0);
        for (int i = 0; i < INITIAL_PHYLOS; i++) {
                add_philosopher();
        }
        sem_post(forks_sem);

        int kb = 1;
        while (kb) {
                kb = readKeyboard();
        }

        for (int i = 0; i < n_philosophers_table; i++) {
                remove_philosopher();
        }

        sem_close(forks_sem);
        sem_close(sem_philosophers_table);
        sem_close(sem_print);

        return 0;
}
