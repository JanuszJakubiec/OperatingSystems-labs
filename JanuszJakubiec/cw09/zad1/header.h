#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <bits/sigaction.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <pthread.h>

#define ELVES_N 10
#define SANTA_ELVES_TRIGGER 3
#define REINDEER_N 9
#define SANTA_REINDEER_TRIGGER 9

#define ELVES_WORK_MIN 2
#define ELVES_WORK_MAX 15
#define REINDEER_HOLIDAY_MIN 5
#define REINDEER_HOLIDAY_MAX 10
#define GIFTS_DELIVERY_MIN 2
#define GIFTS_DELIVERY_MAX 4
#define SANTA_PROBLEM_SOLVING_MIN 1
#define SANTA_PROBLEM_SOLVING_MAX 2







//
