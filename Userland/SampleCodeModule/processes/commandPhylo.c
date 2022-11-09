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

#define PHYLO_EAT_AT_LEAST_FOR 1
#define PHYLO_EAT_AT_MOST_FOR 3
#define PHYLO_THINK_AT_LEAST_FOR 3
#define PHYLO_THINK_AT_MOST_FOR 5

#define TABLE_MAX_PHYLOS_PER_ROWS 5
#define TABLE_PRINT_SLEEP 5

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

int print_table(int argc, char **argv)
{
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return 1;
        }

        while (1) {
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
                printf("\n===============\n");

                sem_post(sem_print);

                sleep(TABLE_PRINT_SLEEP);
        }

        return 0;
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

int philosopher(int argc, char **argv)
{
        const int index = atoi(argv[1]);
        phylo_t *phil = &philosophers_table[index];
        sem_t *forks_sem = sem_open(SEM_PHYLO_NAME, 0);
        if (forks_sem == NULL) {
                printf("Failed: forks_sem\n");
                return 1;
        }

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return 1;
        }

        phil->state = THINKING;
        sem_init_bin(&phil->both_forks_available, 0);

        sem_wait(sem_print);
        printf("A new philosopher has arrived!\n"
               "There are %d philosophers.\n",
               n_philosophers_table);
        sem_post(sem_print);

        while (phil->state != DEAD) {
                int think_time = PHYLO_THINK_AT_LEAST_FOR +
                                 rand() % PHYLO_THINK_AT_MOST_FOR;
                sleep(think_time);

                // Take forks
                sem_wait(forks_sem);
                phil->state = HUNGRY;
                try_forks(index);
                sem_post(forks_sem);

                // Block if it could not acquire forks
                sem_wait(&phil->both_forks_available);

                // Eat
                int eat_time =
                        PHYLO_EAT_AT_LEAST_FOR + rand() % PHYLO_EAT_AT_MOST_FOR;
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

static void print_table_in_background()
{
        char *argv_print_table[] = { "print_table", 0 };
        createProcess(argv_print_table[0], print_table, 0, argv_print_table, 0);
}

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
        int remove = 0;

        do {
                remove = rand() % MAX_PHYLOS;
        } while (philosophers_table[remove].state == DEAD);

        phylo_t *phil = &philosophers_table[remove];

        phil->state = DEAD;
        sem_destroy(&phil->both_forks_available);

        n_philosophers_table -= 1;

        sem_wait(sem_print);
        printf("Removed philosopher number %d\n"
               "There are %d philosophers left\n",
               remove + 1, n_philosophers_table);
        sem_post(sem_print);

        sem_post(sem_philosophers_table);
}

static void add_philosopher()
{
        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 0);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return;
        }

        int argc = 3;
        char **argv = calloc(3, sizeof(char *));

        sem_wait(sem_philosophers_table);
        n_philosophers_table += 1;

        argv[0] = calloc(strlen("philosopher") + 1, sizeof(char));
        memcpy(argv[0], "philosopher", strlen("philosopher"));

        argv[1] = calloc(16, sizeof(char));
        intToString(n_philosophers_table, argv[1]);

        argv[2] = 0;

        createProcess(argv[0], &philosopher, argc, argv, 0);
        sem_post(sem_philosophers_table);
}

static int readKeyboard()
{
        int c = getChar();
        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 0);
        if (sem_print == NULL) {
                printf("Failed: sem_print\n");
                return 0;
        }

        switch (c) {
        case 'a':
                if (n_philosophers_table < MAX_PHYLOS) {
                        add_philosopher();
                } else {
                        sem_wait(sem_print);
                        printf("Table full.\n");
                        sem_post(sem_print);
                }
                break;
        case 'r':

                if (n_philosophers_table > MIN_PHYLOS) {
                        remove_philosopher();
                } else {
                        sem_wait(sem_print);
                        printf("It would be rude to live your friends "
                               "eating alone.\n"
                               "(There are only %d philosophers)\n",
                               n_philosophers_table);
                        sem_post(sem_print);
                }
                break;
        case 'q':
                sem_post(sem_print);
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

        // ""Random seed"". This gives us some predictability
        srand(38403);
        n_philosophers_table = 0;

        sem_t *sem_philosophers_table = sem_open(SEM_TABLE_NAME, 1);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_philosophers_table\n");
                return 1;
        }

        sem_t *sem_print = sem_open(SEM_PRINT_NAME, 1);
        if (sem_philosophers_table == NULL) {
                printf("Failed: sem_print\n");
                return 1;
        }

        sem_t *forks_sem = sem_open(SEM_PHYLO_NAME, 0);
        if (forks_sem == NULL) {
                printf("Failed: forks_sem\n");
                return 1;
        }

        for (int i = 0; i < INITIAL_PHYLOS; i++) {
                add_philosopher();
        }
        sem_post(forks_sem);

        print_table_in_background();

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
